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

#include "spi_robocol.h"

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
spi_st spi_start(char* ruta, int speed)
{
	uint8_t bits=8;
	uint8_t mode=0;
	spi_fd = open(ruta, O_RDWR);
	if (spi_fd < 0)
	{
		perror("Error openning file");
		return -1;
	}

	/*
	 * Mode
	 */
	if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode)== -1)
	{
		perror("can't set spi mode");
		return SPI_ERROR;
	}
	
	if (ioctl(spi_fd, SPI_IOC_RD_MODE, &mode) == -1)
	{
		perror("can't get spi mode");
		return SPI_ERROR;
	}

	/*
	 * bits per word
	 */
	if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1){
		perror("can't set bits per word");
		return -1;
	}
	if (ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1){
		perror("can't set bits per word");
		return -1;
	}

	/*
	 * max speed hz
	 */
	if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
	{
		perror("Error setting speed");
		return -1;
	}

	if (ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
	{
		perror("Error setting speed");
		return -1;
	}


	return 0;
}

/*
** ===================================================================
**     Método      :  spi_setup
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
spi_st spi_create_device(spi_device* dev, uint8_t pMode, uint8_t pPin){

	// Configuración de pin CS como salida
	gpio_set_dir(pPin, OUT);
	// Valor del CS en 1, dejando desactivado el dispositivo
	gpio_gal_value(pPin, 1);

	//Creación de dispositivo SPI virtual
	spi_device new={pMode,pPin,8,8,0};
	*dev=new;

	return SPI_OK;
}


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

spi_st spi_rw(spi_device* device, uint8_t* data_out, uint8_t* data_in, int length){


	// Habilitando comunicación con el dispositivo
	gpio_gal_value((*device).pin,LOW);

	// Configura modo de operación según dispositivo
	if (ioctl(spi_fd, SPI_IOC_WR_MODE, &((*device).mode))== -1)
	{
		perror("can't set spi mode");
		return SPI_ERROR;
	}
	if (ioctl(spi_fd, SPI_IOC_RD_MODE, &((*device).mode)) == -1)
	{
		perror("can't get spi mode");
		return SPI_ERROR;
	}	

	//
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)data_out,
		.rx_buf = (unsigned long)data_in,
		.len = length,
		.bits_per_word=(*device).rd_bpw,
	};

	if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 1)
	{
		perror("Error in transfer");
		return SPI_ERROR;
	}

	// Terminando comunicación con el dispositivo
	gpio_gal_value((*device).pin,HIGH);

	return SPI_OK;
}

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

spi_st spi_write(spi_device* device, char* data_out,  int length)
{
	// Habilitando comunicación con el dispositivo
	gpio_gal_value((*device).pin,LOW);

	// Set spi mode
	if (ioctl(spi_fd, SPI_IOC_WR_MODE, &((*device).mode)) == -1){
		perror("can't set spi mode");
		return SPI_ERROR;
	}

	// Crea la estructura de mesaje 
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)data_out,
		.len = length,
		.bits_per_word=(*device).wr_bpw,
	};

	// Operación de escritura por SPI
	if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 1)
	{
		perror("Error while writting");
		return SPI_ERROR;
	}

	return SPI_OK;
}

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

spi_st spi_read(spi_device* device, char* data_in, int length)
{
	// Habilitando comunicación con el dispositivo
	gpio_gal_value((*device).pin,LOW);

	// Set spi mode
	if (ioctl(spi_fd, SPI_IOC_RD_MODE, &((*device).mode)) == -1)
	{
		perror("can't get spi mode");
		return SPI_ERROR;
	}

	// Crea la estructura de mesaje 
	struct spi_ioc_transfer tr = {
		.rx_buf = (unsigned long)data_in,
		.len = length,
		.bits_per_word=(*device).rd_bpw,
	};

	// Crea la estructura de mesaj
	if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 1)
	{
		perror("Error while reading");
		return SPI_ERROR;
	}
	// Terminando comunicación con el dispositivo
	gpio_gal_value((*device).pin,HIGH);

	return SPI_OK;
}


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
spi_st spi_rd_bpw(spi_device* dev, uint8_t bits)
{
	(*dev).rd_bpw=bits;
	return SPI_OK;
}

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
spi_st spi_wr_bpw(spi_device* dev, uint8_t bits)
{
	(*dev).wr_bpw=bits;
	return SPI_OK;
}

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
spi_st spi_delay(spi_device* dev, uint8_t delay)
{
	(*dev).delay=delay;
	return SPI_OK;
}

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
spi_st spi_mode(spi_device* dev, uint8_t mode)
{
	(*dev).mode=mode;
	return SPI_OK;
}