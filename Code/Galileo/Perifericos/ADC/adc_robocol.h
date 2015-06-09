/*
 * --------------------------------------------------------------------------------------
 * adc_robocol.h 1.0
 *
 *  Created on: 2015
 *
 *	Authors: Robocol
 *
 * -------------------------------------------------------------------------------------
 */
#ifndef ADC_ROBOCOL_H
#define ADC_ROBOCOL_H

 /*
** =====================================================================================
**     USO DEL DRIVER
*/
/*!
**     @resumen
**			Este driver facilita la lectura de los valores resultantes del funcionamiento
**			de los ADC de la tarjeta Intel Galileo Gen1 ante una exitación por una señal 
**			de voltaje. Se vale de las prestaciones que posee el sistema operativo linux 
**			mediante la escritura de archivos de configuracion para determinar la habilitación 
**			de los ADC. La obtencion de los valores resultantes del ADC  se realiza de manera
**			similar, mediante la lectura de archivos destinados por el sistema operativo para
**			que dichos perifericos escriban el resultado de operacion.
**
/* =====================================================================================*/

/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */
#include "gpio_robocol.h"

/*--------------------------------------------------------------------------*/
/*
 *         	  DEFINICIÓN DE VARIABLES DE TERMINACION DE METODOS
 */
static const uint8_t ADC_ERROR=1;
static const uint8_t ADC_OK=0;
typedef uint8_t adc_st;

/*--------------------------------------------------------------------------*/
/*
 * DEFINICIÓN DE VARIABLES CORRESPONDIENTES A LOS PINES PARA HABILITACION DE
 *									ADC
 */
static const uint8_t AD0 =37;
static const uint8_t AD1 =36;
static const uint8_t AD2 =23;
static const uint8_t AD3 =22;
static const uint8_t AD4 =21;
static const uint8_t AD5 =20;
static const uint8_t GPIO29 =29;

/*--------------------------------------------------------------------------*/
/*
 *					DEFINICION DE ESTRUCTURA ADC_DEV
 */
typedef struct adc_dev{
	uint8_t adc_num;
	uint16_t* data;
	float v_ref;
	int fd;
}adc_dev;



/*
** ===================================================================
**     Método      :  build_adc
*/
/*!
**     @resumen
**				Realiza la habilitación para funcionamiento del ADC con
**				el número especificado en el apuntador a la estructura de
**  	        adc_dev pasada por parámetro.
**     @param
**         devptr  - Puntero hacia estructura adc_dev que contiene
**					 el file descriptor, numero y buffer de lectura del
**				     ADC.
**
*/
/* ===================================================================*/
adc_st build_adc(adc_dev* devptr);

/*
** ===================================================================
**     Método      :  get_adc_value
*/
/*!
**     @resumen
**				   - Obtiene el último valor leído por el ADC
**     @param
**         devptr  - Puntero hacia estructura adc_dev que contiene
**					 el file descriptor, numero y buffer de lectura del
**				     ADC.
*/
/* ===================================================================*/
adc_st get_adc_value(adc_dev* devptr);

adc_st get_volatge(adc_dev *devptr,float* voltage);




/*
** ===================================================================
**     Método      :  parse_adc_gpio
*/
/*!
**     @resumen
**				   - Realiza una conversión entre el número de ADC pasado
**					 por parámetro y el correspondiente GPIO de la galileo
**     @param
**			adc_num 
**				   - Numero de ADC en la Galileo. Variable con rango permitido
**					 de 0 a 5.
**
**			gpio_adc 
**				   - Puntero que refiere al lugar donde se almacena el
**					 número de GPIO asociado con el número de ADC pasado por 
**					 parámetro.
*/
/* ===================================================================*/
static adc_st parse_adc_gpio(uint8_t adc_num ,uint8_t* gpio_adc );


/*
** ===================================================================
**     Método      :  parse_adc_route
/*!
**     @resumen
**				   - Realiza una conversión entre el número de ADC pasado
**					 por parámetro y un char array que contiene la 
**					 ruta de lectura para la obtención del valor del ADC.
**     @param
**			adc_num 
**				   - Numero de ADC en la Galileo. Variable con rango permitido
**					 de 0 a 5.
**
**			route_adc 
**				   - Puntero que refiere al lugar donde se almacena el
**					 la ruta par lectura  asociada con el número de ADC 
**					 pasado por parámetro.
*/
/* ===================================================================*/
static adc_st parse_adc_route(uint8_t adc_num ,char* route_adc );


/*(UTIL SI SE MEJORA LA MANERA DE OBTENER LOS DATOS DEL ADC, i.e. ,
** no cerrando y abriendo el archivo de lectura cada vez que se obtiene
** el dato del dispositivo)
** ===================================================================
**     Método      :  close_adc
/*!
**     @resumen
**				Cierra el file descriptor asociado con el archivo de
**				lectura del valor del ADC.
**     @param
**         devptr  - Puntero hacia estructura adc_dev que contiene
**					 el file descriptor, numero y buffer de lectura del
**				     ADC.
**
/* ===================================================================*/
adc_st close_adc(adc_dev* devptr);

//TODO
adc_st digital_filter(void);

#endif 