/*
 * --------------------------------------------------------------------------------------
 * spi_robocol.c (0.5)
 *
 *  Created on: Feb 12, 2015
 *
 *      Authors:	Daniel Ochoa 
 *				    Germán Giraldo
 * -------------------------------------------------------------------------------------
 */


#ifndef SPI_ROBOCOL_H
#define SPI_ROBOCOL_H

 
/*
** =====================================================================================
**     USO DEL DRIVER
*/
/*!
**     @resumen
**          Este driver facilita el manejo de una interfaz de comunicación SPI desdes 
**			userspace en las tarjetas Intel Galileo. Se soporta en el driver standard de
**		 	linux para dispositivos SPI descrito en "linux/spi/spi-dev.h", y en la 
**			llamada del sistema ioctl() que se encarga del manejo de periféricos desde 
**			userspace. Además, se permite el manejo de múltiples dispositivos SPI 
**			simultaneamente aprovechando los GPIO de la Galileo como CS
**			
**			El funcionamiento de este driver se basa en dispositivos virtuales que
**			representan dispositivos reales con los cuales se desea communicar. Los 
**			dispositivos virtuales son estructuras de datos que contienen la información 
**			de configuración necesaria para la comunicación con el dispositivo real al que 
**			representa. Estos dispositivos virtuales pueden ser creados y modificados 
**			usando las funciones definidas en el driver, aunque tambien pueden modificarse 
**			manualmente si así lo desea el usuario.  A continuación se describen las 
**			opciones de configuración con las que se cuenta
**
**          mode 			- Modo de operación del dispositivo SPI. Su valor depende de
**							la polaridad del clock (CPOL) y de la fase para la lectura (CPHA).
**							Un clock con polaridad 0 se mantiene en bajo hasta el momento de 
**							operación. Un clock con fase de lectura 0, determina que se lee 
**							en el primer flanco despues del reposo y no en el segundo (CPHA=1)
**							Opciones:
**								0: CPOL=0; CPHA=0;
**								1: CPOL=0; CPHA=1;
**								2: CPOL=1; CPHA=0;
**								3: CPOL=1; CPHA=1;
** 							este atributo puede modificarse con la función spi_mode()
**
**			pin 			Pin de la tarjeta Galileo por el cual se enviará la señal de CS.
**
**			rd_bpw 			Bits por palabra recibida por SPI.
**
**			wr_bpw 			Bits por palabra enviada por SPI.
**
**			cs_change 		Define si se resetea el dispositivo entre rtansferencias múltiples.
**								0:=no hay reset;   1:=Hay reset;
**			
**			delay 			iempo de espera en ms entre dos transacciones consecutivas.
**     @param
**          speed	     	- Frecuencia del clock
**     @return
**                         	- Estado salida del método. 
*/
/* =====================================================================================*/


/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include "gpio_robocol.h"

/*--------------------------------------------------------------------------*/
/*
 *                  DEFINICIÓN DE TYPES
 */
	typedef int spi_st;

	int spi_fd;


	typedef struct {
		uint8_t mode;
		uint8_t pin;
		uint8_t rd_bpw;
		uint8_t wr_bpw;
		uint8_t delay;
	} spi_device;


#define SPI_ERROR	0x01
#define SPI_OK 		0x00


/* =====================================================================================*/
/*
						DEFINICIÓN DE FUNCIONES 

Nota: Esta librería no garantiza que el gpio que está asociado al pin se encuentre 
conectado por los multiplexores de selección. Para configurar adecuadamente estos 
multiplexores, referirse al esquema de conexión de los GPIO par ala Intel Galileo Gen1.
 */
/* =====================================================================================*/


/*
** ===================================================================
**     Método      :  spi_setup
*/
/*!
**     @resumen
**          Realiza una operación de lectura y escritura full duplex 
**			a través del dispositivo spi especificado.
**     @param
**          ruta     	   	- Ruta absoluta del dispositivo SPI
**     @param
**          speed	     	- Frecuencia del clock
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
spi_st spi_start(char* ruta, int speed);

/*
** ===================================================================
**     Método      :  spi_create_device
*/
/*!
**     @resumen
**          Realiza una operación de lectura y escritura full duplex 
**			a través del dispositivo spi especificado. Entre la 
**			operación de escritura y posterior lectura ocurre sin que 
**			se desactive el dispositivo entre ellas.
**     @param
**          ruta     	   	- Ruta absoluta del dispositivo SPI
**     @param
**          speed	     	- Frecuencia del clock
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
//spi_device* spi_create_device(uint8_t pMode, uint8_t pPin);
spi_st spi_create_device(spi_device* dev, uint8_t pMode, uint8_t pPin);

/*
** ===================================================================
**     Método      :  spi_rw
*/
/*!
**     @resumen
**          Realiza una operación de lectura y escritura full duplex 
**			a través del dispositivo spi especificado. 
**     @param
**          device     	   	- Puntero al dispositivo a traves del cual  
**							se debe realizar la operación.
**     @param
**          data_out 		- Puntero a los datos que se van a escribir
**     @param
**          data_in	     	- Puntero al buffer de almacenamiento de
** 							los datos de entrada. 
**	   @param
**			length			- Tamaño de data_in y data_out en bytes
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
spi_st spi_rw(spi_device* device, uint8_t* data_out, uint8_t* data_in, int length);

/*
** ===================================================================
**     Método      :  spi_write
*/
/*!
**     @resumen
**          Realiza una operación de escritura a través del dispositivo 
**			spi especificado.
**     @param
**          device     	   	- Dispositivo a traves del cual se debe 
**							realizar la operación.
**     @param
**          data_out 		- Puntero a los datos que se van a escribir
**	   @param
**			length			- Tamaño de data_in y data_out en bytes
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/

spi_st spi_write(spi_device* device, char* data_out,  int length);

/*
** ===================================================================
**     Método      :  spi_read
*/
/*!
**     @resumen
**          Realiza una operación de lectura a través del dispositivo 
**			spi especificado. 
**     @param
**          device     	   	- Dispositivo a traves del cual se debe 
**							realizar la operación.
**     @param
**          data_out 		- Puntero a los datos que se van a escribir
**     @param
**          data_in	     	- Puntero al buffer de almacenamiento de
** 							los datos de entrada. Puede ser el mismo
**							data_out.
**	   @param
**			length			- Tamaño de data_in y data_out en bytes
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
spi_st spi_read(spi_device* device, char* data_in, int length);

/*
** ===================================================================
**     Método      :  spi_rd_bpw
*/
/*!
**     @resumen
**          Modifica el número de bits de cada palabra leida por SPI
**     @param
**          dev     	   	- Puntero al dispositivo a traves del cual 
**							se debe	realizar la operación.
**     @param
**          bits     	   	- Bits por palabra leida.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
spi_st spi_rd_bpw(spi_device* dev, uint8_t bits);

/*
** ===================================================================
**     Método      :  spi_wr_bpw
*/
/*!
**     @resumen
**          Modifica el número de bits de cada palabra escrita por SPI
**     @param
**          dev     	   	- Puntero al dispositivo a traves del cual 
**							se debe	realizar la operación.
**     @param
**          bits     	   	- Bits por palabra leida.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
spi_st spi_wr_bpw(spi_device* dev, uint8_t bits);

/*
** ===================================================================
**     Método      :  spi_delay
*/
/*!
**     @resumen
**          Modifica el delay entre mensajes consecutivos cuando se usa
**			la función transferencia múltiple.
**     @param
**          dev     	   	- Puntero al dispositivo a traves del cual 
**							se debe	realizar la operación.
**     @param
**          bits     	   	- Bits por palabra leida.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
spi_st spi_delay(spi_device* dev, uint8_t delay);

/*
** ===================================================================
**     Método      :  spi_mode
*/
/*!
**     @resumen
**          Modifica modo de operación del dispositivo SPI
**     @param
**          dev     	   	- Puntero al dispositivo a traves del cual 
**							se debe	realizar la operación.
**     @param
**          mode     	   	- Nuevo modo de operación.
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
spi_st spi_mode(spi_device* dev, uint8_t mode);

#endif		/**/