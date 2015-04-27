/*
 * i2c_robocol.h
 *
 *  Created on: 	Feb-Mar  2015
 *  Authors:		ROBOCOL
 *				    
 */
#ifndef I2CROBOCOL_H
#define I2CROBOCOL_H
/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */
#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

/*--------------------------------------------------------------------------*/
/*
 *                 CONSTANTES DE LA LIBRERIA
 */
#define I2C_ERROR 		0x01;
#define I2C_OK 			0x10;

/*--------------------------------------------------------------------------*/
/*
 *                 VARIABLES DE LA LIBRERIA
 */	

typedef unsigned char i2c_st;
struct i2c_rdwr_ioctl_data i2c_msgs;
struct i2c_msg i2c_message_in;
struct i2c_msg i2c_message_out;
struct i2c_rdwr_ioctl_data i2c_msgs;


/*---------------------------------------------------
/*                                                  /*
* 				Estructura I2c						*
/*													/*
*---------------------------------------------------*/
typedef struct i2c_device
{
	uint8_t addr;
	int fd;
	struct i2c_rdwr_ioctl_data i2c_msgs;
	char i2c_buff_in;
	char i2c_buff_out;	
}i2c_device;
struct i2c_device dev;

/*--------------------------------------------------------------------------*/
/*
 *                 MÉTODOS DE LA LIBRERIA
 */

/*
** ===================================================================
**     Método      :  i2c_open
*/
/*!
**     @resumen
**			Inicializa el adaptador de comunicación i2c y configura comunicación
**			por default con el dispositivo asociado a la dirección pasada por
**			parámetro.			
**     @param
**			path: Ruta de la "ubicación" en sistema del adaptador de dispositivo i2c.
**				  para el caso de la galileo "/dev/i2c-0".
**			addr: Dirección del dispositivo hardware con el cual se realiza 
**				  conexión.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_open(const char* path , uint8_t addr);

/*
** ===================================================================
**     Método      :  i2c_close
*/
/*!
**     @resumen
**          Cierra el file descriptor asociado con el adaptador de comunicación i2c
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/

i2c_st i2c_close(void);

/*
** ===================================================================
**     Método      :  i2c_read
*/
/*!
**     @resumen
**			Realiza lectura del último dispositivo direccionado en la estructura
**			i2c_device y almacena datos leidos en el apuntador pasado por parámetro.		
**     @param
**			data: Apuntador del buffer en el cual se quiere guarda el dato leido
**			size: Tamaño de la lectura deseada.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_read(char* data, int size);

/*
** ===================================================================
**     Método      :  i2c_write
*/
/*!
**     @resumen
**			Realiza escritura hacia el último dispositivo direccionado en la estructura
**			i2c_device. Se envian los datos direccionados por el apuntador pasado por
**			paŕametro.		
**     @param
**			data: Apuntador al espacio en memoria que contiene la instrucción o información
**				a enviar hacia el dispositivo cuya dirección se encuentra almacenada en el 
**				i2c_device dev.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_write(char* data);


/*
** ===================================================================
**     Método      :  i2c_write_rw
*/
/*!
**     @resumen
**			Realiza escritura hacia el último dispositivo direccionado en la estructura
**			i2c_device. Se envian los datos direccionados por el apuntador pasado por
**			paŕametro. NOTA:Esta es una operación de lectura/escritura combinada sin pausa
**          intermedia. Usar para comunicación con dispositivos que solo permiten este modo
**          de transacción y posteriormente obtener los datos leidos con i2c_read_rw. Valido
**			unicamente si el adaptador tiene I2C_FUNC_I2C(Plain i2c-level commands).
**			
**     @param
**			data: Apuntador al espacio en memoria que contiene la instrucción o información
**				a enviar hacia el dispositivo cuya dirección se encuentra almacenada en el 
**				i2c_device dev.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_write_rw(char* data);

/*
** ===================================================================
**     Método      :  i2c_write_rw
*/
/*!
**     @resumen
**			Realiza lectura de información almacenada en dev.i2c_buff_in. Uso tipico despues de
**			i2c_write_rw.
**			
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_read_rw(char* buff);

/*
** ===================================================================
**     Método      :  i2c_funcs
*/
/*!
**     @resumen
**		     Obtiene la funcionalidad del adaptador y la almacena en *func.
**     @param
**			func: Apuntador hacia el espacio en el cual se desea almacenar
**				la información de funcionalidad del adaptador i2c.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_funcs(unsigned long* func);


/*
** ===================================================================
**     Método      :  i2c_chaddr
*/
/*!
**     @resumen
**			Modifica el address del dispositivo y lo almacena en el espacio
**			dedicado para el mismo en el dispositivo i2c_device dev.
**     @param
**			addr: Dirección del dispositivo hardware con el cual se realiza 
**				  conexión.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_chaddr(uint8_t addr);


#endif /* I2CROBOCOL_H */

//////TODO:Probar método change address