/*
 * PI_motores.cpp
 *
 * Created: 31/01/2015 16:08:40
 *  Author: FTAVSAN
 */
#define F_CPU 4000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/************************************************************************/
/* Definiciones                                                         */
/************************************************************************/

// Definiciones de estado
#define AUTO_EST 0x10
#define MANUAL_EST 0x20
#define FAIL_EST 0x30

// Definiciones de GPIO
#define Sense_PIN PORTA0
#define VEL_PIN PORTA1
#define INA_PIN PORTA2
#define LED_PIN PORTA3
#define PWM_PIN PORTA7
#define PWM_DIR_REG DDRA
#define PWM_REG PORTA
#define FAULT_PIN PORTB2

// Definiciones de SPI
#define USI_OUT_REG	PORTA	// USI port output register.
#define USI_DIR_REG	DDRA	// USI port direction register.
#define USI_CLOCK_PIN	PORTA4	// USI clock I/O pin.
#define USI_DATAIN_PIN	PORTA6	// USI data input pin.
#define USI_DATAOUT_PIN	PORTA5	// USI data output pin.

//Definciones Protocolo de mensajes

#define MEDIR_CORRIENTE 0x01
#define MEDIR_VELOCIDAD 0x02
#define CAMBIAR_KP_V	0x03
#define CAMBIAR_KI_V	0x04
#define CAMBIAR_KP_C	0x05
#define CAMBIAR_KI_C	0x06
#define CAMBIAR_ESTADO	0x07
#define SP_CORRIENTE	0x08
#define SP_VELOCIDAD	0x09
#define CAMBIAR_PWM		0x0A
#define DAR_ESTADO		0x0B
#define MEDIR_TEMP		0x0C

#define ACK_galileo		0xAA
#define ACK_tiny		0xBB
#define ERROR_tiny		0x11

/************************************************************************/
/* Variables Globales                                                   */
/************************************************************************/
unsigned char ESTADO;
bool esBrazo=false;

bool primero =true;
bool parsear=false;
unsigned char encabezado;
unsigned char argumento;

float KP_V = 1.08; //Ku=2.7
float KI_V = 0.0340; //Tu=0.00304*14
int16_t errorAnt_V = 0;
float U_V = 0;
uint8_t spVel =0;
uint8_t velAct=0;
unsigned int countQDEC;

float KP_C = 0.1040;//0.2080;  //Ku=0.52
float KI_C = 0;//0.0067;//0.0134;//Tu=0.0083
int16_t errorAnt_C = 0;
int8_t U_C = 0; //char porque esencialmente es un PWM
uint8_t spCur =0;
uint8_t curAct=0;

unsigned char tempAct;

uint8_t PWM=0;

/************************************************************************/
/* Registro de buffer de entrada
 Los bytes de entrada son guardados en este byte hasta que la transferencia
 este completa. Toma copia del registro USIDR en cada lectura.
 */
/************************************************************************/
unsigned char storedUSIDR;

/************************************************************************/
/* Estructura con elestado del driver
 Esta estructura contiene banderas para el manejo del driver.
 */
/************************************************************************/
struct usidriverStatus_t {
	unsigned char masterMode : 1; // True if in master mode.
	unsigned char transferComplete : 1; // True when transfer completed.
	unsigned char writeCollision : 1; // True if put attempted during transfer.
};

/************************************************************************/
/* Instancia de las banderas de estado

 */
/************************************************************************/
volatile struct usidriverStatus_t spi_status; // The driver status bits.


/************************************************************************/
/* Metodos de configuracion                                             */
/************************************************************************/

/************************************************************************/
/* Blink Led. Revisado 25 Feb
 */
/************************************************************************/
void gpio_blinkLed() {
	PORTA |= (1 << LED_PIN);
	int i=10000;
	while (i>0){
		i--;
	}
	PORTA &= ~(1 << LED_PIN);
	i=10000;
	while (i>0){
		i--;
	}
}
//Revisado 25Feb
bool gpio_put(unsigned char pinOut, unsigned char val) {
	//Asserts
	if (pinOut>=8||val>=2) {
		return 0;
	}
	//Codigo
	if(val)
		PORTA |= (1<< pinOut);
	else
		PORTA &=~(1<<pinOut);
	return 1;
}

bool gpio_get(unsigned char pinIn) {
	if (PORTA & pinIn) {
		return 1;
	} else {
		return 0;
	}
}

/************************************************************************/
/* Inicializa la funcionalidad PWM en "Fast PWM" en el PORTB2
 @return bool TRUE si la inicializacion se logra, false de lo contrario
 */
/************************************************************************/
bool pwm_init() {
	// Inicializa el PORTB2 como pin de
	PWM_DIR_REG |= (1<<PWM_PIN);
	PWM_REG |= (1<<PWM_PIN);
	//Se cambia la configuracion del reloj
	cli();
	// No es necesario poner los otros en 0 ya que el Fuse CKDIV8 se apaga
	// i.e. el valor del vector CLKPR es 0x00 por defecto
	CLKPR |= (1<<CLKPCE); 
	CLKPR = 0b00000001;
	
	sei();

	//DDRB |= (1 << DDB2);
	//PORTB |= (1 << PORTB2);

	// Inicializa el PWM en Fast PWM y con pin de salida en B2
	TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

	// Configura la frecuencia de oscilacion:    Freq 500Hz @ 1MHz     y     Freq 2kHz @ 4MHz
	//TCCR0B |= (1 << CS01); //Prescaler = 8
	TCCR0B |= (1<<CS00) ;
	// Configura el ciclo util inicial en 0%
	OCR0B = 0x00;

	return true;
}
/************************************************************************/
/* Inicializa la funcionalidad de decodificacion de encoder por cuadratura
 @return bool True si la inicialización fue exitosa, false de lo contrario
 */
/************************************************************************/
void qdec_init() {
	cli();

	/////////////////////////////////////////////
	//			Configuracion del timer
	////////////////////////////////////////////

	// Habilita las interrupciones por comparacion
	TIMSK1 |= (1 << OCIE1A);

	// Establece un prescaler de 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// Establece el valor de comparacion
	OCR1AL = 100;

	/////////////////////////////////////////////
	//		Configuracion de interrupcion externa
	////////////////////////////////////////////

	//Habilita la interrupcion externa
	DDRB &= ~(1 << DDA1);
	//PORTA |= (1 << PORTA1);

	PCMSK0 |= (1<<PCINT1);
	GIMSK |= (1<<PCIE0);
	sei();
}

unsigned char qdec_getSpeed() {
	if (countQDEC>255) {
		countQDEC=255;
	}
	return countQDEC;
}

/* Configuración de la interrupción INT0*/
void init_INT0(void){
	DDRB |=0<<PB2;  // Se configura PB2 como entrada

	MCUCR|=1<<ISC00;  //Se configura el tipo de cambio que hace saltar la interrupción 11->Rising
	MCUCR|=1<<ISC01;

	GIMSK|=(1<<INT0); //Se cambia la máscara la interrupción del pin INT0
}

// ISR(EXT_INT0_vect)
// {
// 	PORTA |=(1<<PA0); // PA0 Alto
// 	
// }

ISR(PCINT1_vect)
{
	cli();

	// Hace un conteo de un pulso de la senal index
	countQDEC++;
	sei();
}

ISR(TIM1_COMPA_vect)
{
	cli();
	// Calcula la velocidad angular
	velAct = countQDEC; // -> countQDEC/25;
	// Resetea el timer
	countQDEC = 0;
	TCNT1L = 0;

	sei();
}

ISR(USI_OVF_vect)
{
	cli();
	// Update flags and clear USI counter
	USISR = (1<<USIOIF);
	spi_status.transferComplete = 1;
	// Copy USIDR to buffer to prevent overwrite on next transfer.
	storedUSIDR = USIDR;
	
	if(primero){
		encabezado =storedUSIDR;
		if(encabezado == MEDIR_CORRIENTE || encabezado == MEDIR_TEMP || encabezado==MEDIR_VELOCIDAD || encabezado==DAR_ESTADO){
			parsear = true;
		}
		primero=false;
	}
	else{
		argumento=storedUSIDR;
		parsear=true;
		primero=true;
	}
	
	USIDR = ACK_tiny;
	
	sei();
}
/************************************************************************/
/* Inicializa la USI como un esclavo SPI.
 Esta funcion configura la direccion de los pines y el modulo
 */
/************************************************************************/
void spi_initslave(char spi_mode) {
	// Configure port directions.
	USI_DIR_REG |= (1<<USI_DATAOUT_PIN);
	// Outputs.
	USI_DIR_REG &= ~(1<<USI_DATAIN_PIN) | (1<<USI_CLOCK_PIN);
	// Inputs.
	USI_OUT_REG |= (1<<USI_DATAIN_PIN) | (1<<USI_CLOCK_PIN);
	// Pull-ups.

	// Configure USI to 3-wire slave mode with overflow interrupt.
	USICR = (1<<USIOIE) | (1<<USIWM0) | (1<<USICS1) | (spi_mode<<USICS0);

	// Init driver status register.
	spi_status.masterMode = 0;
	spi_status.transferComplete = 0;
	spi_status.writeCollision = 0;
	USIDR = ACK_tiny;
	storedUSIDR = 0;
}

/************************************************************************/
/* Pone un bit en el bus.
 Esta funcion escribe un byte en el USIDR. En modo esclavo este registro
 prepara la informacion que sera enviada al maestro, cuando este indique
 una comunicacion
 @return 0 si ocurre una colision, 1 de lo contrario
 */
/************************************************************************/
char spi_put(unsigned char val) {
	// Check if transmission in progress,
	// i.e. USI counter unequal to zero.
	if ( (USISR & 0x0F) != 0) {
		// Indicate write collision and return.
		spi_status.writeCollision = 1;
		return 0;
	}

	// Reinit flags.
	spi_status.transferComplete = 0;
	spi_status.writeCollision = 0;

	// Put data in USI data register.
	USIDR = val;

	if (spi_status.writeCollision == 0)
		return 1;
	return 0;
}

/************************************************************************/
/* Obtiene un byte del bus
 Este metodo se limita a retornar el valor de stored USIDR
 */
/************************************************************************/
unsigned char spi_get() {
	return storedUSIDR;
}

/************************************************************************/
/* Espera una transferencia completa
 Este metodo se asegura que la ejecucion del programa este ocupada hasta
 que ocurra la interrupcion de transferencia completa
 */
/************************************************************************/
void spi_wait() {
	do {
	} while (spi_status.transferComplete == 0);
}
/************************************************************************/
/* Configura ADC.
 */
/************************************************************************/
void adc_init() {
	//PA0
	PRR&=~(1<<PRADC);
	ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS2);
	//ADCSRB|=(1<ADLAR);//Configura la lectura de ADC alineada a la izquierda (8bits de precision)
}

unsigned int adc_get() {
	unsigned int lectura=0;
	ADCSRA|=(1<<ADSC);
	while ((ADCSRA & (1<<ADIF)) != 0x10);
	//Espera conversion completa
	lectura = ADC;
	ADCSRA |=(1 << ADIF);
	return lectura;
}

/************************************************************************/
/* Configura los pines GPIO
 //TODO*/
/************************************************************************/
void gpio_init() {

	DDRA|=(1<<LED_PIN); //Salidas
	DDRA&=~((1<<Sense_PIN)|(1<<INA_PIN)|(1<<VEL_PIN)|(1<<FAULT_PIN)); //Entradas
}

int control_getActualSpeed(void) {
	return velAct;
}

int control_piSpeed(void) {
	int16_t setPointCorriente;
	float deltaU_V, k1, k2;

	k1 = KP_V + KI_V;
	k2 = -KP_V;
	int16_t error = spVel- velAct;
	deltaU_V = k1 * error + k2 * errorAnt_V;

	U_V = U_V + deltaU_V;

	if (U_V > 255) {
		U_V = 255;
	} else if (U_V < 0) {
		U_V = 0;
	}

	setPointCorriente = U_V;
	errorAnt_V = error;

	return setPointCorriente;
}

void control_getActualCurrent(void) {
	ADMUX=0;
	curAct=adc_get();
}

void control_getTemp(void){
	ADMUX=0b00100010;
	tempAct= adc_get();
}

uint8_t control_piCurrent(void) {
	int8_t cicloUtil;
	float k1, k2, deltaU_C;

	k1 = KP_C + KI_C;
	k2 = -KP_C;
	int16_t error = spCur-curAct;
	deltaU_C = k1 * error + k2 * errorAnt_C;

	U_C = U_C + deltaU_C;

	if (U_C > 250) {
		U_C = 250;
	} else if (U_C < 1) {
		U_C = 1;
	}

	cicloUtil = U_C;
	errorAnt_C = error;

	return cicloUtil;
}

//!-------------------------------------------------------------------------------
//!	@brief Modifica el ciclo util del PWM
//!
//!	@param Ciclo util en escala de 0 - 255
//!
//!	@return nothing
//!-------------------------------------------------------------------------------
void pwm_set(uint8_t PWM) {
	if(PWM>0x7F && esBrazo){
		PWM=0x7F;
	}
	OCR0B = PWM;
}


unsigned char debug(unsigned char msg) {
	spi_put(msg);
	spi_wait();
	return spi_get();
}

void control_parser(){
	if(parsear){
		switch (encabezado){
			case MEDIR_CORRIENTE:
			control_getActualCurrent();
			spi_put(curAct);
			break;
			case MEDIR_VELOCIDAD:
			control_getActualSpeed();
			spi_put(velAct);
			break;
			case MEDIR_TEMP:
			control_getTemp();
			spi_put(tempAct);
			break;
			case DAR_ESTADO:
			spi_put(ESTADO);
			break;
			case SP_CORRIENTE:
			spCur=argumento;
			break;
			case SP_VELOCIDAD:
			spVel=argumento;
			break;
			case CAMBIAR_KP_V:
			KP_V=argumento;
			break;
			case CAMBIAR_KI_V:
			KI_V=argumento;
			break;
			case CAMBIAR_KP_C:
			KP_C=argumento;
			break;
			case CAMBIAR_KI_C:
			KI_C=argumento;
			break;
			case CAMBIAR_ESTADO:
			ESTADO=argumento;
			break;
			case CAMBIAR_PWM:
			PWM=argumento;
			break;
		}
		parsear=false;
	}		
}

int main(void) {
	int iC=0;
	gpio_init();
	adc_init();
	pwm_init();
	qdec_init();
	spi_initslave(0);
	pwm_set(0);
	ESTADO=MANUAL_EST;
	curAct=0;
	spCur=0;
	sei();
	while (1) {
		control_parser();
		switch (ESTADO) {
		case AUTO_EST:
			control_getActualSpeed();
			control_getActualCurrent();	
			PWM=control_piCurrent();
			pwm_set(PWM);
			gpio_put(LED_PIN,0);
			break;
		case MANUAL_EST:
			//spCur=127;
			gpio_put(LED_PIN,1);
			pwm_set(PWM);
			break;
		case FAIL_EST:
			pwm_set(0);
			errorAnt_C=0;
			errorAnt_V=0;
			spi_put(ERROR_tiny);
			storedUSIDR=0x00;
			break;
		default:
			cli();
			gpio_put(LED_PIN, 1);
			_delay_ms(10);
			gpio_put(LED_PIN, 0);
			_delay_ms(10);
			
			gpio_put(LED_PIN, 1);
			_delay_ms(10);
			gpio_put(LED_PIN, 0);
			_delay_ms(10);
			
			gpio_put(LED_PIN, 1);
			_delay_ms(10);
			gpio_put(LED_PIN, 0);
			_delay_ms(10);
			sei();
			break;
		}
	}
};
