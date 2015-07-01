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
#include <stdint.h>

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
#define CAMBIAR_PWM	0x0A
#define DAR_ESTADO	0x0B
#define MEDIR_TEMP	0x0C
#define MEDIR_PWM	0x0D

//Valores a transmitir por SPI que no contienen información propiamente
#define ACK_galileo	0xAA
#define ACK_tiny	0xBB
#define ERROR_tiny	0x11


/************************************************************************/
/*				VARIABLES                             */
/************************************************************************/

// Estado del microcontrolador
unsigned char ESTADO;

// Variables de control de comunicación    
                
bool primero =true;	//Indica que es el primer byte recibido
bool parsear=false;	//Traduce la orden en una acción
unsigned char encabezado; //Almacena la orden que envió la galileo
unsigned char argumento;  //Almacena el mensaje que se quiere enviar

// Estructura con el estado del driver: Esta estructura contiene banderas para el manejo del driver.
struct usidriverStatus_t {
	unsigned char masterMode : 1; // True if in master mode.
	unsigned char transferComplete : 1; // True when transfer completed.
	unsigned char writeCollision : 1; // True if put attempted during transfer.
};

// Instancia de las banderas de estado
volatile struct usidriverStatus_t spi_status; // The driver status bits.


// ConstantesControlador Velocidad
float KP_V = 1.36;//Ku=7
float KI_V = 1.36;//0.0340;//Tu=2

float KD_V = 0.9;//0.0340;//Tu=0.00304*14
int16_t  errorAnterior_V=0;
int16_t  errorAnteriorAnterior_V=0;
float UAnterior_V=0; //Acción de control anterior (sp_corriente)
uint8_t setPointVelocidad=30;
uint8_t velAct=0; //Medición de posición para el brazo

uint16_t countQDEC; //Almacena el conteo de pulsos que llegan del encoder



// Contantes Controlador  de Corriente
float KP_C =0.036;//40:0.06;//0.24;//0.104;//0.1040;//Ku=0.26=1.95
float KI_C =0.15428;//40:0.19;//346;//346.38;//0.0067;//Tu=0.0083=0.0034
int16_t  errorAnterior_C=0;
float  UAnterior_C=0; //Acción de control anterior (PWM)
uint8_t setPointCorriente=0;
uint8_t curAct=0;

// Temperatura interna del microcontrolador
uint8_t tempAct;

// Ciclo útil
uint8_t PWM=0;

// Registro de buffer de entrada:
// Los bytes de entrada son guardados en este byte hasta que la transferencia
// esté completa. Toma copia del registro USIDR en cada lectura.
unsigned char storedUSIDR;


/************************************************************************
 			MÉTODOS DEL MICROCONTROLADOR
*************************************************************************/


/************************************************************************/
/* Hace parpadear el LED rojo
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
	if (PINA & pinIn) {
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
	CLKPR |= (1<<CLKPCE); 
	CLKPR = 0b00000001;
	

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


/************************************************************************
 			CONFIGURACIÓN DE INTERRUPCIONES
*************************************************************************
************************************************************************ */


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
	ADCSRB=0x10;//Configura la lectura de ADC alineada a la izquierda (8bits de precision)
}

unsigned char adc_get() {
	unsigned char lectura=0;
	ADCSRA|=(1<<ADSC);
	while ((ADCSRA & (1<<ADIF)) != 0x10);
	//Espera conversion completa
	lectura = ADCH;
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

void control_getActualCurrent(void) {
	ADMUX=0;
	curAct=adc_get();
}

void control_getTemp(void){
	ADMUX=0b00100010;
	tempAct= adc_get();
}

/*********************************************************************************************************
** Function name: bloquePi0
** Descriptions: ejecuta el controlador PI
** input parameters:error Es el error actual, error1 el error anterior, y lectura ADC sensor
** Returned value: setPointPWM
*********************************************************************************************************/
uint8_t bloquePiVelocidad(int16_t error){
	int16_t setPointCorriente;
	float deltaU_V,k1,k2,k3;	

	//Cálculo de constantes para utilizar la fórmula de Euler
	k1 = KP_V + KI_V + KD_V;
	k2 = -KP_V - 2*KD_V;
	k3 = KD_V;


	deltaU_V = k1*error +k2*errorAnterior_V +k3*errorAnteriorAnterior_V;
	
	UAnterior_V= UAnterior_V+deltaU_V;
	setPointCorriente= UAnterior_V;

	//Satura la acción de control para garantizar un máximo de 7.44 A
	if(setPointCorriente>100){
		setPointCorriente=100;
		UAnterior_V=100;
	}	
	else if(setPointCorriente<0){
		setPointCorriente=0;
		UAnterior_V=0;
	}
	
	//Adapta las ganancias dependiendo del setpoint de corriente
/*	if(setPointCorriente<15){
		KP_C =0.18;
		KI_C =0.7714;
	}
	else if(setPointCorriente<20){
		KP_C =0.108;
		KI_C =0.46284;
	}
	else if(setPointCorriente<40){
		KP_C =0.036;
		KI_C =0.15428;
	}
	else if(setPointCorriente<60){
		KP_C =0.0144;
		KI_C =0.061712;
	}
	else if(setPointCorriente<80){
		KP_C =0.0036;
		KI_C =0.015428;
	}
	else if(setPointCorriente<100){
		KP_C =0.00108;
		KI_C =0.00462;
	}
	else if(setPointCorriente<120){
		KP_C =0.000242;
		KI_C =0.000924;
	}
	else if(setPointCorriente<140){
		KP_C =0.000108;
		KI_C =0.000462;
	}
	else if(setPointCorriente<160){
		KP_C =0.000059;
		KI_C =0.000231;
	}
*/
	
	errorAnteriorAnterior_V=errorAnterior_V;
	errorAnterior_V=error;
	
	
	return setPointCorriente;
}

/*********************************************************************************************************
** Function name: bloquePi1
** Descriptions: ejecuta el controlador PI
** input parameters:error Es el error actual, error1 el error anterior
** Returned value: cicloUtil para el PWM
** Se demora 42ms
*********************************************************************************************************/
uint8_t bloquePiCorriente(int16_t error){
	uint8_t cicloUtil;	
	float k1,k2,deltaU_C;	
		
	k1 = KP_C+KI_C; 
	k2 = -KP_C;
	
	deltaU_C = k1*error +k2*errorAnterior_C;

	if(deltaU_C>255){
		deltaU_C=255;
	}else if(deltaU_C<-255){
		deltaU_C=-255;
	}
	
	
	UAnterior_C=UAnterior_C+deltaU_C;
	cicloUtil=UAnterior_C;
	
	if(cicloUtil>255){
		cicloUtil=250;
		UAnterior_C=250;
	}	
	else if(cicloUtil<1){
		cicloUtil=1;
		UAnterior_C=1;
	}	
	

	errorAnterior_C=error;
	
	
	return cicloUtil;
}


/************************************************************************/
/* Inicializa la funcionalidad de decodificacion de encoder por cuadratura
 @return bool True si la inicialización fue exitosa, false de lo contrario
 */
/************************************************************************/
void qdec_init(void) {
	cli();

	/////////////////////////////////////////////
	//			Configuracion del timer
	////////////////////////////////////////////

	// Habilita las interrupciones por comparacion
	TIMSK1 |= (1 << OCIE1A);

	// Establece un prescaler de 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// Establece el valor de comparacion
	OCR1AL = 10;

	/////////////////////////////////////////////
	//		Configuracion de interrupcion externa
	////////////////////////////////////////////

	//Habilita la interrupcion externa
	DDRA &= ~(1 << PINA1);
	PORTA |= (1 << PORTA1);

	PCMSK0 |= (1<<PCINT1);
	GIMSK |= (1<<PCIE0);
	sei();
}

/************************************************************************/
/* Inicializa la funcionalidad de decodificacion de encoder por cuadratura
 @return bool True si la inicialización fue exitosa, false de lo contrario
 */
/************************************************************************/
ISR(PCINT0_vect)
{
	//cli();
	// Hace un conteo de un pulso de la senhal del encoder
	gpio_put(LED_PIN,1);
	countQDEC++;
	//sei();
}

/************************************************************************/
/* Inicializa la funcionalidad de decodificacion de encoder por cuadratura
 @return bool True si la inicialización fue exitosa, false de lo contrario
 */
/************************************************************************/
ISR(TIM1_COMPA_vect)
{
	//cli();
	// Calcula la velocidad angular
	

	if (countQDEC>255) {
		countQDEC=255;
	}
	velAct = countQDEC; // -> countQDEC/25;

	// Resetea el timer
	countQDEC = 0;
	TCNT1L = 0;


	//sei();
}

void qdec_off()
{
	TIMSK1 &= ~(1 << OCIE1A);
	PCMSK0 &= ~(1 << PCINT1);
	countQDEC = 0;
	TCNT1L = 0;
}

void qdec_on()
{
	TIMSK1 |= (1 << OCIE1A);
	PCMSK0 |= (1 << PCINT1);
}

//!-------------------------------------------------------------------------------
//!	@brief Modifica el ciclo util del PWM
//!
//!	@param Ciclo util en escala de 0 - 255
//!
//!	@return nothing
//!-------------------------------------------------------------------------------
void pwm_set(uint8_t PWM) {
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
			//control_getActualCurrent();
			spi_put(curAct);
			break;
			case MEDIR_VELOCIDAD:
			spi_put(velAct);
			break;
			case MEDIR_TEMP:
			control_getTemp();
			spi_put(setPointCorriente);
//			spi_put(tempAct);
			break;
			case DAR_ESTADO:
			spi_put(ESTADO);
			break;
			case SP_CORRIENTE:
			setPointCorriente=argumento;
			break;
			case SP_VELOCIDAD:
			setPointVelocidad=argumento;
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

// Interrupción de recepción por SPI
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
			control_parser();
		}
		primero=false;
	}
	else{
		argumento=storedUSIDR;
		parsear=true;
		control_parser();
		primero=true;
	}

	
	
	//USIDR = ACK_tiny;
	
	sei();
}

int main(void) {
	uint8_t i;	
	int16_t error_velocidad=0;
	int16_t error_corriente=0;
	gpio_init();
	adc_init();
	pwm_init();
	spi_initslave(0);
	pwm_set(0);
	qdec_init();
	ESTADO=MANUAL_EST;

	sei();
	while (1) {
		
		switch (ESTADO) {
		case AUTO_EST:
			gpio_put(LED_PIN,0);

			
			//Mide velocidad y actualiza velAct			
			qdec_on();			
			_delay_ms(5);
			qdec_off();

			//Cálculo del error de velocidad
			error_velocidad=setPointVelocidad-velAct;		
			setPointCorriente=bloquePiVelocidad(error_velocidad);
			for(i=0;i<6;i++){			
				control_getActualCurrent();
				error_corriente=setPointCorriente-curAct;
				PWM=bloquePiCorriente(error_corriente);
				pwm_set(PWM);
				_delay_ms(12);
			}

			break;
		case MANUAL_EST:
			//Mide velocidad y actualiza velAct			
			qdec_on();			
			_delay_ms(5);
			qdec_off();
			
			_delay_ms(10);
			gpio_put(LED_PIN,1);
			pwm_set(PWM);
			break;
		case FAIL_EST:
			pwm_set(0);
			errorAnterior_C=0;
			errorAnterior_V=0;
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
