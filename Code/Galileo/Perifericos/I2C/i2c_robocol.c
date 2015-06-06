/*
 * i2c_robocol.c
 *
 *  Created on:     Feb-Mar  2015
 *  Authors:        ROBOCOL
 ** ===================================================================
 *  Libreria para el manejo de i2c 
 ** ===================================================================
 */
#include "i2c_robocol.h"

/*
** ===================================================================
**     Método      :  i2c_open
*/
/*!
**     @resumen
**          Inicializa el adaptador de comunicación i2c y configura comunicación
**          por default con el dispositivo asociado a la dirección pasada por
**          parámetro.          
**     @param
**          path: Ruta de la "ubicación" en sistema del adaptador de dispositivo i2c.
**                para el caso de la galileo "/dev/i2c-0".
**          addr: Dirección del dispositivo hardware con el cual se realiza 
**                conexión.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_open(const char* path, uint8_t addr ){

    if (((dev).fd = open(path,O_RDWR))<0) {
       printf("Failed to open the bus.\n");
       perror("Error");
       return I2C_ERROR;
    }else{
        printf("Success openning the bus.\n");
    }

    if (ioctl((dev).fd,I2C_SLAVE,addr) < 0) {
        printf("Failed to set address for talking with slave\n");
        perror("Error");
        return I2C_ERROR;
    }
    dev.addr=addr;
    i2c_message_out=(struct i2c_msg){dev.addr,0x00,1,&dev.i2c_buff_out};
    i2c_message_in=(struct i2c_msg){dev.addr,0x01,1,&dev.i2c_buff_in};

	return I2C_OK;
}


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

i2c_st i2c_close(void){
    if(close(dev.fd)<0){
        return I2C_ERROR;
    }
    return I2C_OK;
}

/*
** ===================================================================
**     Método      :  i2c_read
*/
/*!
**     @resumen
**          Realiza lectura del último dispositivo direccionado en la estructura
**          i2c_device y almacena datos leidos en el apuntador pasado por parámetro.        
**     @param
**          data: Apuntador del buffer en el cual se quiere guarda el dato leido
**          size: Tamaño de la lectura deseada.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_read(char* data ,int size){

    if (ioctl((dev).fd,I2C_SLAVE,(dev).addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        perror("Error");
        return I2C_ERROR;
    }
    // Using I2C Read
    if (read((dev).fd,data,size) != 1) {
       printf("Failed to read from the i2c bus.\n");
       perror("Error");
       return I2C_ERROR;
    } 
    return I2C_OK;
}
/*
** ===================================================================
**     Método      :  i2c_write
*/
/*!
**     @resumen
**          Realiza escritura hacia el último dispositivo direccionado en la estructura
**          i2c_device. Se envian los datos direccionados por el apuntador pasado por
**          paŕametro.      
**     @param
**          data: Apuntador al espacio en memoria que contiene la instrucción o información
**              a enviar hacia el dispositivo cuya dirección se encuentra almacenada en el 
**              i2c_device dev.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_write(char* data){
    int c;
	int size=0;
	char* aux=data;

    if (ioctl((dev).fd,I2C_SLAVE,(dev).addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        perror("Error: ");
        return I2C_ERROR;
    }

	while(*aux!='\0'){
		size++;
		aux++;
	}
    size=2;
    if ((c=write((dev).fd,data,size)) != size) {
	/* ERROR HANDLING: i2c transaction failed */
	   printf("Failed to write transaction the i2c bus.\n");
        return I2C_ERROR;
    }
    return I2C_OK;


}

/*
** ===================================================================
**     Método      :  i2c_write_rw
*/
/*!
**     @resumen
**          Realiza escritura hacia el último dispositivo direccionado en la estructura
**          i2c_device. Se envian los datos direccionados por el apuntador pasado por
**          paŕametro. NOTA:Esta es una operación de lectura/escritura combinada sin pausa
**          intermedia. Usar para comunicación con dispositivos que solo permiten este modo
**          de transacción y posteriormente obtener los datos leidos con i2c_read_rw. Valido
**          unicamente si el adaptador tiene I2C_FUNC_I2C(Plain i2c-level commands).
**          
**     @param
**          data: Apuntador al espacio en memoria que contiene la instrucción o información
**              a enviar hacia el dispositivo cuya dirección se encuentra almacenada en el 
**              i2c_device dev.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_write_rw(char* data){
    dev.i2c_buff_out=*data;

    if (ioctl((dev).fd,I2C_SLAVE,(dev).addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        perror("Error: ");
        return I2C_ERROR;
    }


    struct i2c_msg mensajes[2]={i2c_message_in,i2c_message_out};
    i2c_msgs.msgs=mensajes;
    i2c_msgs.nmsgs=2;
    dev.i2c_msgs=i2c_msgs;

    if(ioctl(dev.fd,I2C_RDWR,&(dev.i2c_msgs))<0){
        printf("Failed to write\n");
        perror("Error");
        return I2C_ERROR;
    }
    return I2C_OK;
        
}       

/*
** ===================================================================
**     Método      :  i2c_write_rw
*/
/*!
**     @resumen
**          Realiza lectura de información almacenada en dev.i2c_buff_in. Uso tipico despues de
**          i2c_write_rw.
**          
**     @return
**          Apuntador hacia el dev.i2c_buff_in para obtener información almacenada desde última
**          transacción.
*/
/* ===================================================================*/
 i2c_st i2c_read_rw(char* buff){
    *buff=dev.i2c_buff_in;
    return I2C_OK;

}


/*
** ===================================================================
**     Método      :  i2c_funcs
*/
/*!
**     @resumen
**           Obtiene la funcionalidad del adaptador y la almacena en *func.
**     @param
**          func: Apuntador hacia el espacio en el cual se desea almacenar
**              la información de funcionalidad del adaptador i2c.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_funcs( unsigned long* funcs){
	unsigned long func;
	if((func=ioctl((dev).fd, I2C_FUNCS,*funcs))<0){
    	printf("Failed to acquire bus access and/or talk to slave.\n");
        perror("Error: ");
		return I2C_ERROR;
	}
	return I2C_OK;
}

/*
** ===================================================================
**     Método      :  i2c_chaddr
*/
/*!
**     @resumen
**          Modifica el address del dispositivo y lo almacena en el espacio
**          dedicado para el mismo en el dispositivo i2c_device dev.
**     @param
**          addr: Dirección del dispositivo hardware con el cual se realiza 
**                conexión.
**     @return
**          I2C_OK si la operaracion fue exitosa, I2C_ERROR de lo contrario.
*/
/* ===================================================================*/
i2c_st i2c_chaddr(uint8_t addr){

    if (ioctl((dev).fd,I2C_SLAVE,addr) < 0) {
        printf("Failed to change address.\n");
        perror("Error");
        return I2C_ERROR;
    }
    dev.addr=addr;
    i2c_message_out=(struct i2c_msg){dev.addr,0x00,1,&dev.i2c_buff_out};
    i2c_message_in=(struct i2c_msg){dev.addr,0x01,1,&dev.i2c_buff_in};
    return I2C_OK;
}