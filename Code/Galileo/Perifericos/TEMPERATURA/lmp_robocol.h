#ifndef LMP_ROBOCOL_H
#define LMP_ROBOCOL_H

#include "spi_robocol.h"
#include "misc_robocol.h"

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

CH_SCAN_SEL	chanel scan select
		0x0(Default) Scan Mode0: Single-Chanel continuos conversion
LAST_CH		last channel for conversion
		0x6(default) ch6
FIRST_CH	Starting chanel for conversion
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
 *                ESTADOS DE LA COMUNICIÓN CON EL LMP90100
 *
 * Estas definiciones contienen los valuees posibles de st_lmp.
 */
#define LMP_OK		0x00U
#define LMP_ESPERA 	0x01U
#define LMP_ERROR	0x02U

/*Transaction 1-URA Setup*/
#define WRITE_ADDR 0x10
#define READ_ADDR  0x90
#define FILLER_T1  0x0
/*Transaction 2-Data Access*/
#define WRITE 0
#define READ  1
#define FILLER_T2 0
#define LOWADDR_SL 0b00001111
 /**Size Transaction 2*/
#define SZ1 0x0 /*1 Byte*/
#define SZ2 0x1 /*2 Bytes*/
#define SZ3 0x2 /*3 Bytes*/
#define SZS 0x3 /**Streaming 3+ Bytes hasta que se desactive CS*/

typedef uint8_t st_lmp;

typedef struct{
	uint8_t pin_cs;		//
	uint8_t	pin_interr;
	uint8_t address;
	spi_device* spi;    //No es necesario que el usuario instancie este campo.
}lmp_dev;

st_lmp build_lmp(lmp_dev* dev);

st_lmp write_reg_lmp(lmp_dev* dev, uint8_t address,uint8_t* data,uint8_t num);

// st_lmp write_io_lmp(uint8_t bit, uint8_t* value);

st_lmp read_reg_lmp(lmp_dev* dev,uint8_t address,uint8_t* data, uint8_t num);

st_lmp power_lmp(lmp_dev* dev,uint8_t mode);

st_lmp config_DRYB_lmp(lmp_dev* dev);

st_lmp read_ADC_lmp(lmp_dev* dev, uint32_t* lectura);

st_lmp parse_byteSize(uint8_t num, uint8_t* sz);


#endif /* LMP_H_ */
