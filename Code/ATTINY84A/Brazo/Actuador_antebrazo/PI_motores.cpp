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
#define CAMBIAR_KP_P	0x03
#define CAMBIAR_KI_P	0x04
#define CAMBIAR_KP_C	0x05
#define CAMBIAR_KI_C	0x06
#define CAMBIAR_ESTADO	0x07
#define SP_CORRIENTE	0x08
#define SP_POSICION	0x09
#define CAMBIAR_PWM	0x0A
#define DAR_ESTADO	0x0B
#define MEDIR_TEMP	0x0C

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

//Puntos servocontrol y retardo para ajustar el control
uint8_t servocontrol[20]={100,100,102,103,105,109,113,118,124,130,136,142,147,151,155,157,159,160,160,160};
uint8_t compensacion_temporal=0;


// Constantes Controlador Posición
float KP_P = 0.2;//Ku=2.7 0.2*Ku
float KI_P = 1;//0.0340;//Tu=0.00304*14 "2Kp/Tu
float KD_P = 0.0266;//KpTu/3
int16_t  errorAnterior_P=0;
int16_t  errorAnteriorAnterior_P=0;
float UAnterior_P=0; //Acción de control anterior (sp_corriente)
uint8_t setPointPosicion=155;
uint8_t PosAct=0; //Medición de posición para el brazo


// Contantes Controlador  de Corriente
float KP_C =0.00045;//0.1125;//40:0.06;//0.24;//0.104;//0.1040;//Ku=0.26=1.95
float KI_C =0.0015;//40:0.19;//346;//346.38;//0.0067;//Tu=0.0083=0.0034
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

/*********************************************************************************************************
** Function name: spi_wait
** Descriptions: Espera a que la transmisión por SPI esté completa, evitando que se acceda al registro del
** puerto cuando una transmisión está en proceso.
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void spi_wait() {
	do {
	} while (spi_status.transferComplete == 0);
}

/*********************************************************************************************************
** Function name: adc_init
** Descriptions: Inicializa el ADC: Canal, tiempo de carga del condensador de S&H, Alineación de la 
** respuesta
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void adc_init() {
	//PA0
	PRR&=~(1<<PRADC);
	ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS2);
	ADCSRB=0x10;//Configura la lectura de ADC alineada a la izquierda (8bits de precision)
}

/*********************************************************************************************************
** Function name: gpio_init
** Descriptions: Inicializa los pines a utilizar en la operación del uC. El LED que indica el estado es
** una salida, los pines de ADC, Falla e INA (Dirección) son entradas
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
unsigned char adc_get() {
	unsigned char lectura=0;
	ADCSRA|=(1<<ADSC);
	while ((ADCSRA & (1<<ADIF)) != 0x10);
	//Espera conversion completa
	lectura = ADCH;
	ADCSRA |=(1 << ADIF);
	return lectura;
}

/*********************************************************************************************************
** Function name: gpio_init
** Descriptions: Inicializa los pines a utilizar en la operación del uC. El LED que indica el estado es
** una salida, los pines de ADC, Falla e INA (Dirección) son entradas
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void gpio_init() {
	DDRA|=(1<<LED_PIN); //Salidas
	DDRA&=~(1<<DDA2);
	DDRA&=~(1<<Sense_PIN);
	DDRA&=~(1<<VEL_PIN);
	DDRA&=~(1<<FAULT_PIN);
	
//	DDRA&=~((1<<Sense_PIN)|(1<<INA_PIN)|(1<<VEL_PIN)|(1<<FAULT_PIN)); //Entradas
}

/*********************************************************************************************************
** Function name: control_getActualPosition
** Descriptions: Actualiza la variable global PosAct que contiene la posición del actuador
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void control_getActualPosition(void) {
	ADMUX=0x01;
	PosAct=adc_get();	
}

/*********************************************************************************************************
** Function name: control_getActualCurrent
** Descriptions: Actualiza la variable global curAct que contiene la corriente consumida por el actuador
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void control_getActualCurrent(void) {
	ADMUX=0;
	curAct=adc_get();
}

/*********************************************************************************************************
** Function name: gpio_init
** Descriptions: Inicializa los pines a utilizar en la operación del uC. El LED que indica el estado es
** una salida, los pines de ADC, Falla e INA (Dirección) son entradas
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void control_getTemp(void){
	ADMUX=0b00100010;
	tempAct= adc_get();
}

/*********************************************************************************************************
** Function name: bloquePidPosicion
** Descriptions: ejecuta un controlador PID de posicion, las ganancias y algunos errores son variables 
** globales
** input parameters: error - Es el error actual de posicion
**		     direccion - Es la direccion del actuador
** Returned value: setPointCorriente
*********************************************************************************************************/
uint8_t bloquePidPosicion(int16_t error, char direccion){
	int16_t setPointCorriente;
	float deltaU_P,k1,k2,k3;

	if(direccion==1){
		KP_P = 0.2;//Ku=2.7 0.2*Ku
		KI_P = 1;//0.0340;//Tu=0.00304*14 "2Kp/Tu
		KD_P = 0.0266;//KpTu/3	
	}else{
		KP_P = 0.2;//Ku=2.7 0.2*Ku
		KI_P = 0.01;//0.0340;//Tu=0.00304*14 "2Kp/Tu
		KD_P = 0.0266;//KpTu/3	
	}	
	//Cálculo de constantes para utilizar la fórmula de Euler
	k1 = KP_P + KI_P + KD_P;
	k2 = -KP_P - 2*KD_P;
	k3 = KD_P;
	
	//Cálculo de la variación en la acción de control
	deltaU_P = k1*error +k2*errorAnterior_P +k3*errorAnteriorAnterior_P;
	
	//Cálculo de la nueva acción de control
	UAnterior_P= UAnterior_P+deltaU_P;
	setPointCorriente= UAnterior_P;

	if(direccion==1){
		//Límites para la subida sin carga
		if(setPointCorriente>60){
			setPointCorriente=60;
			UAnterior_P=60;
		}

		//Cálculo de las ganancias adaptativas
		if(setPointCorriente<30){
			KP_C =0.45;//0.1125;//40:0.06;//0.24;//0.104;//0.1040;//Ku=0.26=1.95
			KI_C =1.5;//40:0.19;//346;//346.38;//0.0067;//Tu=0.0083=0.0034
		}else if(setPointCorriente<40){
			KP_C =0.00375;//0.1125;//40:0.06;//0.24;//0.104;//0.1040;//Ku=0.26=1.95
			KI_C =0.0125;//40:0.19;//346;//346.38;//0.0067;//Tu=0.0083=0.0034
		}else if(setPointCorriente<60){
			KP_C =0.00075;//0.1125;//40:0.06;//0.24;//0.104;//0.1040;//Ku=0.26=1.95
			KI_C =0.0027;//40:0.19;//346;//346.38;//0.0067;//Tu=0.0083=0.0034
		}else if(setPointCorriente<100){
			KP_C =0.00045;//0.1125;//40:0.06;//0.24;//0.104;//0.1040;//Ku=0.26=1.95
			KI_C =0.0015;//40:0.19;//346;//346.38;//0.0067;//Tu=0.0083=0.0034
		}
	
	}else{
		//Límites para la bajada sin carga
		if(setPointCorriente>20){
			setPointCorriente=20;
			UAnterior_P=20;
		}

		KP_C =0.00375;
		KI_C =0.0125;

	}
	 
	//Límite inferior del setpoint
	if(setPointCorriente<0){
		setPointCorriente=0;
		UAnterior_P=0;
	}

	//Almacenamiento de datos del error para la siguiente iteración del control
	errorAnteriorAnterior_P=errorAnterior_P;
	errorAnterior_P=error;
	
	
	
	return setPointCorriente;
}

/*********************************************************************************************************
** Function name: bloquePiCorriente
** Descriptions: ejecuta el controlador PI
** input parameters: error - Es el error actual de corriente
** Returned value: cicloUtil para el PWM
** Se demora 3.24 ms
*********************************************************************************************************/
uint8_t bloquePiCorriente(int16_t error){
	int8_t cicloUtil;	
	float k1,k2,deltaU_C;	
	
	//Cálculo de constantes para utilizar la fórmula de Euler	
	k1 = KP_C+KI_C; 
	k2 = -KP_C;

	//Cálculo de la variación en la acción de control	
	deltaU_C = k1*error +k2*errorAnterior_C;
	
	//Cálculo de la nueva acción de control
	UAnterior_C=UAnterior_C+deltaU_C;
	cicloUtil=UAnterior_C;
	
	//Límites de la acción de control considerando el Voltaje del actuador
	if(cicloUtil>127){
		cicloUtil=127;
		UAnterior_C=127;
	}	
	else if(cicloUtil<1){
		cicloUtil=1;
		UAnterior_C=1;
	}	
	
	//Almacenamiento de datos del error para la siguiente iteración del control
	errorAnterior_C=error;
	
	
	return cicloUtil;
}

/*********************************************************************************************************
** Function name: pwm_set
** Descriptions: Cambia el ciclo útil del PWM que se genera por OCR0B
** input parameters: PWM deseado (0-255)
** Returned value: NA
*********************************************************************************************************/
void pwm_set(uint8_t PWM) {
	OCR0B = PWM;
}

/*********************************************************************************************************
** Function name: bloqueServoControl
** Descriptions: ejecuta un servocontrol con 20 puntos utilizando el control de posición
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void bloqueServoControl(char direccion){
	uint8_t n,j;
	int16_t error_posicion,error_corriente;
	for(n=0;n<20;n++){
		control_getActualPosition();
		error_posicion = servocontrol[n] - PosAct;
		setPointCorriente=bloquePidPosicion(error_posicion,direccion);
		
		//Se ejecuta el control de corriente durante 90 ms (6 ciclos)
		for(j=0;j<6;j++){
			control_getActualCurrent();
			error_corriente=setPointCorriente-curAct;
			PWM=bloquePiCorriente(error_corriente);
			pwm_set(PWM);
			_delay_ms(12);
		}
	}
	
}



/*********************************************************************************************************
** Function name: debug
** Descriptions: Prueba la comunicación por SPI esperando un eco cuando envía un mensaje al maestro
** input parameters: Mensaje de 1 byte a enviar
** Returned value: Mensaje recibido por SPI
*********************************************************************************************************/
unsigned char debug(unsigned char msg) {
	spi_put(msg);
	spi_wait();
	return spi_get();
}

/*********************************************************************************************************
** Function name: control_parser
** Descriptions: Determina la acción que se debe tomar dependiendo de la orden que recibió el uC por SPI
** input parameters: NA
** Returned value: NA
*********************************************************************************************************/
void control_parser(){
	int8_t error;
	if(parsear){
		switch (encabezado){
			case MEDIR_CORRIENTE:
			control_getActualCurrent();
			spi_put(curAct);
			break;
			case MEDIR_VELOCIDAD:
			//control_getActualPosition();
			spi_put(PosAct);
			break;
			case MEDIR_TEMP:
			error=errorAnterior_P;
//			control_getTemp();
//			spi_put(tempAct);
			spi_put(error);
			break;
			case DAR_ESTADO:
			spi_put(ESTADO);
			break;
			case SP_CORRIENTE:
			setPointCorriente=argumento;
			break;
			case SP_POSICION:
			setPointPosicion=argumento;
			break;
			case CAMBIAR_KP_P:
			KP_P=argumento;
			break;
			case CAMBIAR_KI_P:
			KI_P=argumento;
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
	int16_t error_posicion=0;
	int16_t error_corriente=0;
	uint8_t i;
	char direccion=0x00;
	gpio_init();
	adc_init();
	pwm_init();
	spi_initslave(0);
	pwm_set(0);
	ESTADO=MANUAL_EST;

	sei();
	while (1) {
		control_parser();
		switch (ESTADO) {
		case AUTO_EST:
			control_getActualPosition();
			if((0x04 & PINA)==0){//El actuador baja: ON/OFF en cascada con corriente (lazo interno)
				gpio_put(LED_PIN,0);
				direccion=0;
				error_posicion=PosAct-setPointPosicion;
				setPointCorriente=15;
				KP_C =0.00375;
				KI_C =0.0125;

				if (error_posicion>2){
					for(i=0;i<6;i++){			
						control_getActualCurrent();
						error_corriente=setPointCorriente-curAct;
						PWM=bloquePiCorriente(error_corriente);
						pwm_set(PWM);
						_delay_ms(12);
					}
				}else{
					pwm_set(0);
				}
			}else{//El actuador sube: PID en cascada con corriente (lazo interno)
				gpio_put(LED_PIN,1);	
				direccion=1;
				error_posicion=setPointPosicion-PosAct;		
				setPointCorriente=bloquePidPosicion(error_posicion,direccion);
				for(i=0;i<6;i++){			
					control_getActualCurrent();
					error_corriente=setPointCorriente-curAct;
					PWM=bloquePiCorriente(error_corriente);
					pwm_set(PWM);
					_delay_ms(12);
				}	
			}
			

			break;
		case MANUAL_EST:
			pwm_set(PWM);
			break;
		case FAIL_EST:
			pwm_set(0);
			errorAnterior_C=0;
			errorAnterior_P=0;
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
