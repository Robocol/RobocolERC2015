#ifndef ROBOCOL_LMP_H
#define ROBOCOL_LMP_H

/*--------------------------------------------------------------------------*/
/*
 *                 INCLUYE LAS LIBRERIAS DE MANEJO DEL SPI
 */
#include "spi_robocol.h"



/*--------------------------------------------------------------------------*/
/*
 *                    REGISTROS INTERNOS DEL LMP90100
 *
 * Estas definiciones contienen los nombres de los registros y los bits.
 */

/*
CHx_INPUTCN:
BURNOUT_EN 	0(default) Disable sensor diagnostic current injection for this chanel
		1 Enanble sensor diagnostic
VREF_SEL	0(default) Select VREFP1 and VREFN2
		1 Select VREFP2 and VREFN1
VINP		Positive input select
VINN	 	Negative input select
 */

#define CH0		0x00U
#define CH1		0x02U
#define CH2		0x04U
#define CH3		0x06U
#define CH4		0x08U
#define CH5		0x0AU
#define CH6		0x0CU

#define CHx_INPUTCN 	0x20U
#define BURNOUT_ON 		1
#define BURNOUT_OFF		0
#define VREF_SEL1 		0
#define VREF_SEL2 		1
#define VIN0			0x00U
#define VIN1			0x01U
#define VIN2			0x02U
#define VIN3			0x03U
#define VIN4			0x04U
#define VIN5			0x05U
#define VIN6			0x06U
#define VIN7			0x07U

/*
CHx_CONFIG:
ODR_SEL		ODR select 
		0x7(default) 214.65 SPS
GAIN_SEL	Gain select 
		0x0(default) 1 (FGA OFF)
BUF_EN		Enable/Disable the buffer 
		0(default) include the buffer in the signal path 
 */
#define CHx_CONFIG		0x21U
#define ODR_1_6775 		0x00U
#define ODR_3_355 		0x01U
#define ODR_6_71 		0x02U
#define ODR_13_42 		0x03U
#define ODR_26_83125	0x04U
#define ODR_53_6625		0x05U
#define ODR_107_325		0x06U
#define ODR_214_65 		0x07U

#define FGA_OFF_1		0x00U
#define FGA_OFF_2		0x01U
#define FGA_OFF_4		0x02U
#define FGA_OFF_8		0x03U
#define FGA_ON_16		0x04U
#define FGA_ON_32		0x05U
#define FGA_ON_64		0x06U
#define FGA_ON_128		0x07U
#define BUFF_OFF		1
#define BUFF_ON			0

/*
 * GPIO_DIRCN:
 * GPIO_DIRCNx			GPIO direction control – these bits are used to control the direction of each General Purpose Input/Outputs (GPIO) pins D0 - D6.
 * GPIO_DIRCNx = 0		input
 * GPIO_DIRCNx = 1		output
 */
#define GPIO_DIRCN		0x0EU

#define GPIO_DAT		0x0FU
#define D0				0x01U
#define D1				0x02U
#define D2				0x04U
#define D3				0x08U
#define D4				0x10U
#define D5				0x20U
#define D6				0x40U

/*
Background Calibration Control
BGALIN	0x0 	(default) Calibration off
	0x1	Mode1	offset correction / Gain estimation
	0x2	Mode2	offset correction / Gain correction
	0x3	mode3	offset estimation / Gain estimation
 */
#define BGCALCN			0x10U
#define MODE0			0x00U
#define MODE1			0x01U
#define MODE2 			0x02U
#define MODE3 			0x03U

/*
 * SCALCN:
 * 
 * SCALCN				When written, set SCALCN to:
 * 						0x0 (default): Normal Mode
 * 						0x1: “System Calibration Offset Coefficient Determination” mode
 * 						0x2: “System Calibration Gain Coefficient Determination” mode
 * 						0x3: Reserved
 * 						
 * 						
 * 						When read, this bit indicates the system calibration mode is in:
 * 						0x0: Normal Mode
 * 						0x1: "System Calibration Offset Coefficient Determination" mode
 * 						0x2: "System Calibration Gain Coefficient Determination" mode
 * 						0x3: Reserved
 * 						
 */
#define SCALCN			0x17U

/*
 * CHx_SCAL_OFFSET:
 * 
 * CH0_SCAL_OFFSET		Chanel 0
 * CH1_SCAL_OFFSET		Chanel 1
 * CH2_SCAL_OFFSET		Chanel 2
 * CH2_SCAL_OFFSET		Chanel 3
 */
#define CHx_SCAL_OFFSET 	0x30U

/*
 * CHx_SCAL_GAIN:
 * 
 * CH0_SCAL_GAIN		Chanel 0
 * CH1_SCAL_GAIN		Chanel 1
 * CH2_SCAL_GAIN		Chanel 2
 * CH3_SCAL_GAIN		Chanel 3
*/
#define CHx_SCAL_GAIN		0x33U

/*
 * CHx_SCAL_SCALING:
 * 
 * CH0_SCAL_SCALING		Chanel 0
 * CH1_SCAL_SCALING		Chanel 1
 * CH2_SCAL_SCALING		Chanel 2
 * CH3_SCAL_SCALING		Chanel 3
 */
#define CHx_SCAL_SCALING	0x36U

/*
 * CHx_SCAL_BITS_SELECTOR:
 * 
 * CH0_SCAL_BITS_SELECTOR		Chanel 0
 * CH1_SCAL_BITS_SELECTOR		Chanel 1
 * CH2_SCAL_BITS_SELECTOR		Chanel 2
 * CH3_SCAL_BITS_SELECTOR		Chanel 3
 */
#define CHx_SCAL_BITS_SELECTOR	0x37U

/*
CH_SCAN:

CH_SCAN_SEL	channel scan select
		0x0(Default) Scan Mode0: Single-Chanel continuos conversion
LAST_CH		last channel for conversion
		0x6(default) ch6
FIRST_CH	Starting channel for conversion
		0x0(default) ch0
 */
#define CH_SCAN		0x1FU
#define CH_STS		0x1EU

/*
ADC_AUXCN:
RESET_SYSCAN			the system calibration register
						0(default)
CLK_EXT_DET	External 	clock detection
						0(default)
CLK_SEL					Clock selct - only valid if CLK_EXT_DET = 1
						0(DEFAULT)
RTD_CUR_SEL				Selects RTD current 
						0x0(default) 0uA
 */
#define ADC_AUXCN 	0x12U

/*
 * SENDIAG_FLAGS:
 * 
 * SHORT_THLD_ FLAG			Short Circuit Threshold Flag = 1 when the absolute value of VOUT is within the absolute threshold
 * 							voltage set by SENDIAG_THLDH and SENDIAG_THLDL.
 * 												
 * RAILS_FLAG				Rails Flag = 1 when at least one of the inputs is near rail (VA or GND).
 * 
 * POR_AFT_LST_RD			Power-on-reset after last read = 1 when there was a power-on-reset event since the last time the
 * 							SENDIAG_FLAGS register was read.
 * 							
 * OFLO_FLAGS				Overflow flags
 * 							0x0: Normal operation
 * 							0x1: The modulator was not overranged, but ADC_DOUT got clamped to 0x7f_ffff (positivefullscale) or 0x80_0000 (negative full scale)
 * 							0x2: The modulator was over-ranged (VIN > 1.3*VREF/GAIN)
 * 							0x3: The modulator was over-ranged (VIN < -1.3*VREF/GAIN)
 * 
 * SAMPLED_CH				Channel Number – the sampled channel for ADC_DOUT and SENDIAG_FLAGS.
 */
#define SENDIAG_FLAGS		0x19
#define SENDIAG_THLDH		0x14
#define SENDIAG_THLDL		0x15

#define FLAGS_OK			0x00U
#define SHORT_THLD			0x01U
#define RAILS				0x02U
#define POR_AFT_LST_RD		0x03U
#define OFLO				0x04U

/*
 * POWERCN:
 * 
 *		0x0: Active Mode
 *		0x1: Power-down Mode
 *		0x3: Stand-by Mode
 */
#define POWERCN			0x08U
#define LMP_ON			0x00U
#define LMP_OFF			0x01U
#define LMP_STBY		0x03U

/*
 * SPI_DRDYBCN:
 * 
 * SPI_DRDYB_D6			Enable DRDYB on D6
 * 						0 (default): D6 is a GPIO
 * 						1: D6 = drdyb signal
 * 						
 * CRC_RST				CRC Reset
 * 						0 (default): Enable CRC reset on DRDYB deassertion
 * 						1: Disbale CRC reset on DRDYB deassertion
 * 						
 * FGA_BGCAL			Gain background calibration 
 * 						0 (default): Correct FGA gain error. This is useful only if the device is operating in BgcalMode2and ScanMode2 or ScanMode3.
 *						1: Correct FGA gain error using the last known coefficients.
 *						
 *[0:2]					Default - 0x3 (do not change this value)
 */
#define SPI_DRDYBCN	0x11

/*
 * RESETCN:
 * 
 * 0xC3					Reset the registers to their default value
 */
#define RESETCN			0x00

/*
 * ADC_RESTART:
 * 
 * RESTART					Restart conversion
 * 							1: Restart conversion.
 */
#define ADDRESS_ADC_RESTART			0x0B

/*
 * ADC_DONE:
 * 
 * DT_AVAIL_B				Indicates if new conversion data is available
 * 							0x00 - 0xFE: Available.
 * 							0xFF: Not Available
 */
#define ADC_DONE			0x18

/*
 * ADC_DOUT
 */
#define ADC_DOUT			0x1A


/*--------------------------------------------------------------------------*/
/*
 *		                 COMUNICACION_SPI
 *
 * Estas definiciciones contienen constantes para la realizacion de transacciones
 * por spi
 */

/*Transaction 1-URA Setup*/
#define WRITE_ADDR 0x10
#define READ_ADDR  0x90
#define FILLER_T1  0x0
/*Transaction 2-Data Access*/
#define WRITE 0x00
#define READ  0x10
#define FILLER_T2 0
#define LOWADDR_SL 0b11110000
 /**Size Transaction 2*/
#define SZ1 0x0 /*1 Byte*/
#define SZ2 0x1 /*2 Bytes*/
#define SZ3 0x2 /*3 Bytes*/
#define SZS 0x3 /**Streaming 3+ Bytes hasta que se desactive CS*/

/*--------------------------------------------------------------------------*/
/*
 *                ESTADOS DE LA COMUNICIÓN CON EL LMP90100
 *
 * Estas definiciones contienen los valores posibles de st_lmp.
 */
#define LMP_OK		0x00U
#define LMP_ESPERA 	0x01U
#define LMP_ERROR	0x02U



/* Variables globales*/
extern volatile uint8_t lmp_ocupado;
spi_device spi_dev;
uint8_t tx;
uint8_t rx;


/* Definición de tipos de variables utilizados por la librería */
typedef uint8_t st_lmp;

typedef struct {
	uint8_t	channel;				/* Número de canal */
	uint8_t	burnout;						/* Activación de burnout */
	uint8_t	vref;							/* Selección de Voltaje de referencia */
	uint8_t	vinp;					/* Selección de VINP */
	uint8_t	vinn;					/* Selección de VINN */
	uint8_t	odr_sel;				/* Tasa de muestreo */
	uint8_t	ganancia;				/* Ganancia del canal */
	uint8_t	buffer_enable;					/* Habilitar el buffer */
} LMP_ch_config_struct;                     /* Estructura de configuración */

typedef struct {
	uint8_t	channel;				/* Número de canal */
	uint8_t	burnout;						/* Activación de burnout */
	uint8_t	vref;							/* Selección de Voltaje de referencia */
	uint8_t	vinp;					/* Selección de VINP */
	uint8_t	vinn;					/* Selección de VINN */
	uint8_t	odr_sel;				/* Tasa de muestreo */
	uint8_t	ganancia;				/* Ganancia del canal */
	uint8_t	buffer_enable;					/* Habilitar el buffer */
} LMP_cal_config_struct;                     /* Estructura de configuración */

/*
** ===================================================================
**     Método      :  build_lmp
*/
/*!
**     @resumen
**			Realiza la construccion del dispositivo spi para la comunica-
**			cion con el LMP_90100.
**
**     @param
**          ruta     	   	- Ruta absoluta del dispositivo SPI
**     @param
**          speed	     	- Frecuencia del clock
**		@param
**          cs	     		- Pin de la galileo por el cual se maneja
**							  el chip select del LMP90100
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/

st_lmp build_lmp(char* ruta, uint32_t speed, uint8_t cs);

/*
** ===================================================================
**     Método      :  escribir_reg_lmp
*/
/*!
**     @resumen
**         Escribe cierta cantidad de datos en un registro seleccio-
**         nado. Puede escribir más de un dato de forma secuencial lo
**         cual permite escribir registros aledaños.
**     @param
**         address     	   - Dirección del registro
**     @param
**         data     	   - Puntero a los datos que se van a escribir
**     @param
**         num	     	   - Número de bytes a escribir.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp escribir_reg_lmp(uint8_t address,char* data,uint8_t num);

/*
** ===================================================================
**     Método      :  leer_reg_lmp
*/
/*!
**     @resumen
**         Lee cierta cantidad de datos  un registro seleccionado. 
**         Puede leer más de un dato lo cual permite leer registros 
**         aledaños. 
**     @param
**         address     	   - Dirección del registro
**     @param
**         data     	   - Puntero a la variable donde se van a 
**         					 escribir los datos.
**     @param
**         num	     	   - Número de bytes a leer
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp leer_reg_lmp(uint8_t address,char* data,uint8_t num);


/*
** ===================================================================
**     Método      :  leer_reg_lmp
*/
/*!
**     @resumen
**         Retorna el estado actual de la comunicación con el lmp.
**     @return
**                         - Estado actual de la comuniación. 
*/
/* ===================================================================*/
st_lmp estado_lmp(void);

/*
** ===================================================================
**     Método      :  configurar_lmp
*/
/*!
**     @resumen
**         Configura el LMP90100 según lo requerido por el usuario. 
**     @param
**         setup     	   - Estructura que contiene los parámetros de
**         					 configuración.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp ch_config_lmp(LMP_ch_config_struct* opciones);

/*
** ===================================================================
**     Método      :  io_config_lmp
*/
/*!
**     @resumen
**         Configura los pines de proposito general como salida o como
**         entrada. Si el bit correspondiente es 1, ese bit sera salida
**         de lo contrario ese bit sera entrada. 
**     @param
**         salidas     	   - Se ponen en 1 los bits que funcionaran como 
**         					 salida.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp io_config_lmp(char salidas);


/*
** ===================================================================
**     Método      :  escribir_io_lmp
*/
/*!
**     @resumen
**         Escribe el valor deseado en los puertos digitales. Los bits
**         deben estar habilitados como salidas en el registro GPIO_DIRCN
**         sino la asignación del valor no tendrá efecto.
**     @param
**         salidas     	   - Se coloca el valor del char en los pines
**         					 del puerto paralelo asignados como salida.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp escribir_io_lmp(char salidas);


/*
** ===================================================================
**     Método      :  leer_io_lmp
*/
/*!
**     @resumen
**         Lee el valor del bit indicado y lo devuelve en la dirección
**         indicada.
**     @param
**         bit	     	   - Indica cual es el bit a leer (D0-D6).
**     @param
**         valor     	   - Puntero que guarda el valor del bit en un
**         					 un uint8_t (TRUE=1, FALSE=0).
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp leer_io_lmp(char bit, uint8_t* valor);

/*
** ===================================================================
**     Método      :  config_bkcal_lmp
*/
/*!
**     @resumen
**         Configura las opciones de calibración en segundo plano.
**     @param
**         mode	    	   - Modo de operación de la calibración en
**         					 segundo plano.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_bkcal_lmp(char mode);


/*
** ===================================================================
**     Método      :  ajustar_cero_lmp
*/
/*!
**     @resumen
**         Ajusta el cero llenando los registros de offset. El procedi-
**         miento de ajuste del cero es el siguiente:
**         	
**         	1. Aplicar la condición de cero al canal que se va a ajustar.
**         	Ésta condición se aplica con un patrón de medición.
**         	2. Llamar este método para empezar el ajuste.
**         	3. El sistema entrará en modo de determinación del offset y
**         	llenará los registros CHx_SCAL_OFFSET del canal correspon-
**         	diente.
**         	4. El procedimiento debería repetirse varias veces anotando
**         	los valores obtenidos en cada iteración. Al final, se podrá
**         	determinar la precisión del sensor y el valor promedio del
**         	offset.
**     @param
**         canal    	   - Canal que se va a ajustar (CH0-CH3).
**     @param
**         offset    	   - Offset obtenido después de la calibración.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp ajustar_cero_lmp(uint8_t canal, uint8_t* offset);


/*
** ===================================================================
**     Método      :  ajustar_ganancia_lmp
*/
/*!
**     @resumen
**         Ajusta la ganancia llenando los registros de ganancia. El procedi-
**         miento de ajuste de ganancia es el siguiente:
**         	
**         	1. Ajustar el cero. 
**         	2.Aplicar la condición de referencia al canal que se va a ajustar.
**         	Ésta condición se aplica con un patrón de medición.
**         	2. Llamar este método para empezar el ajuste. Se debe indicar la
**         	ganancia esperada. 
**         	3. El sistema entrará en modo de determinación de ganancia y
**         	llenará los registros CHx_SCAL_GAIN del canal correspondiente.
**         	4. El procedimiento debería repetirse varias veces anotando
**         	los valores obtenidos en cada iteración. Al final, se podrá
**         	determinar la precisión del sensor y el valor promedio de la
**         	ganancia.
**     @param
**         canal    	   - Canal que se va a ajustar (CH0-CH3).
**     @param
**         ganancia_deseada	   - Ganancia deseada antes de la calibración.
**     @param
**         ganancia    	   - Ganancia obtenida después de la calibración.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp ajustar_ganacia_lmp(uint8_t canal, uint8_t ganancia_deseada,uint8_t* ganancia);


/*
** ===================================================================
**     Método      :  post_cal_lmp
*/
/*!
**     @resumen
**         Configura las opciones de scaling y corrimiento.
**     @param
**         canal    	   - Canal que se ajusta.
**     @param
**         scal	    	   - Valor de multiplicación (max 63).
**     @param
**         selector    	   - Valor del corrimiento (max 7).
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp post_cal_lmp(uint8_t canal, char scal, char selector);


/*
** ===================================================================
**     Método      :  config_scan_lmp
*/
/*!
**     @resumen
**         Configura el modo de escaneo además del orden de escaneo si
**         se seleccionaron varios canales.
**     @param
**         mode	    	   - Modo de escaneo (MODE0-MODE3).
**     @param
**         primer_canal	   - Primer canal que se escanea.
**     @param
**         ultimo_canal	   - Último canal que se escanea.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_scan_lmp(char mode, char primer_canal, char ultimo_canal);

/*
** ===================================================================
**     Método      :  config_corr_lmp
*/
/*!
**     @resumen
**         Configura el valor de las fuentes de corriente IB1 e IB2. Los
**         valores van desde 0uA hasta 1000uA.
**     @param
**         corriente   	   - Valor de corriente en multiplos de 100uA.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_corr_lmp(char corriente);

/*
** ===================================================================
**     Método      :  config_umbral_lmp
*/
/*!
**     @resumen
**         Configura el umbral del lmp.
**     @param
**         umbral    	   - Valor del umbral.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_umbral_lmp(uint8_t umbral);

/*
** ===================================================================
**     Método      :  activar_diagnostico_lmp
*/
/*!
**     @resumen
**         Este método activa el modo de escaneo 3 el cual se utiliza
**         para actualizar las banderas de diagnóstico.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp activar_diagnostico_lmp(void);


/*
** ===================================================================
**     Método      :  diagnosticar_lmp
*/
/*!
**     @resumen
**         Realiza un diagnóstico del sensor. Los canales diagnosticados
**         deben tener la corriente de burnout habilitada.
**     @param
**         bandera    	   - Indica cual bandera se activo o devuelve
**         					 FLAGS_OK.
**     @param
**         canal    	   - Canal que está siendo muestreado.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp diagnosticar_lmp(char* bandera,char* canal);

/*
** ===================================================================
**     Método      :  power_lmp
*/
/*!
**     @resumen
**         Cambia el estado de la alimentación del dispositivo.
**     @param
**         mode	    	   - Modo de operación deseado (LMP_STBY,LMP_OFF,
**         					 LMP_ON).
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp power_lmp(char mode);

/*
** ===================================================================
**     Método      :  config_DRYB_lmp
*/
/*!
**     @resumen
**         Configura el pin DRYB para que salga por D6.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp config_DRYB_lmp(void);

/*
** ===================================================================
**     Método      :  resetar_lmp
*/
/*!
**     @resumen
**         Resetea el dispositivo y si es necesario resetea los registros 
**         de calibración.
**     @param
**         todos    	   - Resetea los registros de calibración.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp resetear_lmp(uint8_t todos);

/*
** ===================================================================
**     Método      :  leer_ADC_lmp
*/
/*!
**     @resumen
**         Lee la conversión del ADC.
**     @param
**         lectura    	   - Puntero que almacena la lectura.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp leer_ADC_lmp(int* lectura);
#endif /* ROBOCOL_LMP_H */

