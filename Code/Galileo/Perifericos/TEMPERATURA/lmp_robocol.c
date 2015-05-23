#include "lmp_robocol.h"

st_lmp build_lmp(lmp_dev* dev){

	if(spi_start("/dev/spidev1.0",100000)){
		printf("Error en la inicialización interfaz spi.\n");
		return LMP_ERROR;
	}

	if(spi_create_device( (*dev).spi,0,(*dev).pin_cs)){
		printf("Error en la creación del dispositivo spi para comunicación con LMP \n");
		return LMP_OK
	}

	(*dev).address=0xFF;

	return LMP_OK;
}


/*
** ===================================================================
**     Método      :  write_reg_lmp
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
st_lmp write_reg_lmp(lmp_dev* dev, uint8_t address,uint8_t* data, uint8_t num){

	uint8_t* tx_up=malloc(2*sizeof(uint8_t));
	uint8_t* rx_up=malloc(2*sizeof(uint8_t));
	uint8_t* tx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t* rx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t nshift=num*sizeof(uint8_t);
	uint8_t bytes;

	parse_byteSize(num,&bytes);

	if (((*dev).address>>4)!=(address>>4)){
		//UPPER ADDRESS
		tx_up=(WRITE_ADDR<<8)|(FILLER_T1<<4)|(address>>4);
		if(spi_rw(dev,tx_up,rx_up,2)){
			printf("Error en set de upper Addresscon valor: \tAddr:%x\n",address>>4);
			return LMP_ERROR;
		}
		(*dev).address=address;
	}
	//LOWER ADDRES + DATA
	tx_low=(WRITE<<(nshift+sizeof(uint8_t)-1))|(bytes<<(nshift+5))|FILLER_T1<<(nshift+4)|((address&LOWADDR_SL)<<(nshift))|*data;
	if(spi_rw(dev,tx_low,rx_low,num+1)){
		printf("Error en transacción a registro especificado con lower address pasado por parámetro\n"
			"\tAddr:%x\n\tData:%x",LOWADDR_SL&address,*data);
		return LMP_ERROR;
	}

	return LMP_OK;
}

/*
** ===================================================================
**     Método      :  read_reg_lmp
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
**         num	     	   - Número de bytes a leer.
**     @return
**                         - Estado de la comuniación antes de enviar. 
*/
/* ===================================================================*/
st_lmp read_reg_lmp(lmp_dev* dev,uint8_t address,uint8_t* data,uint8_t num){

	uint8_t* tx_up=malloc(2*sizeof(uint8_t));
	uint8_t* rx_up=malloc(2*sizeof(uint8_t));
	uint8_t* tx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t* rx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t nshift=num*sizeof(uint8_t);
	uint8_t bytes;

	parse_byteSize(num,&bytes);

	if (((*dev).address>>4)!=(address>>4)){
		//UPPER ADDRESS
		tx_up=(WRITE_ADDR<<8)|(FILLER_T1<<4)|(address>>4);
		if(spi_rw(dev,tx_up,rx_up,2)){
			printf("Error en set de upper Addresscon valor: \tAddr:%x\n",address>>4);
			return LMP_ERROR;
		}
		(*dev).address=address;
	}


	//LOWER ADDRES + DATA
	tx_low=(READ<<(nshift+sizeof(uint8_t)-1))|(bytes<<(nshift+5))|FILLER_T1<<(nshift+4)|((address&LOWADDR_SL)<<(nshift));
	if(spi_rw(dev,tx_low,rx_low,num+1)){
		printf("Error en transacción a registro especificado con lower address pasado por parámetro\n"
			"\tAddr:%x\n\tData:%x",LOWADDR_SL&address,*data);
		return LMP_ERROR;
	}

	*data=rx_low;
	return LMP_OK;
}

// /*
// ** ===================================================================
// **     Método      :  escribir_io_lmp
// */
// /*!
// **     @resumen
// **         Escribe el value deseado en los puertos digitales. Los bits
// **         deben estar habilitados como salidas en el registro GPIO_DIRCN
// **         sino la asignación del value no tendrá efecto.
// **     @param
// **         salidas     	   - Se coloca el value del uint8_t en los pines
// **         					 del puerto paralelo asignados como salida.
// **     @return
// **                         - Estado de la comuniación antes de enviar. 
// */
// /* ===================================================================*/
// st_lmp write_io_lmp(uint8_t salidas){
// 	if(lmp_ocupado){
// 		return LMP_ESPERA;
// 	}else{
// 		write_reg_lmp(GPIO_DAT,&salidas,1);				
// 		return LMP_OK;
// 	}
// }


// ** ===================================================================
// **     Método      :  leer_io_lmp

// /*!
// **     @resumen
// **         Lee el value del bit indicado y lo devuelve en la dirección
// **         indicada.
// **     @param
// **         bit	     	   - Indica cual es el bit a leer (D0-D6).
// **     @param
// **         value     	   - Puntero que guarda el value del bit en un
// **         					 un uint8_t (TRUE=1, FALSE=0).
// **     @return
// **                         - Estado de la comuniación antes de enviar. 
// */
// /* ===================================================================*/
// st_lmp read_io_lmp(uint8_t bit, uint8_t* value){
// 	uint8_t registro=NULL;
// 	if(lmp_ocupado){
// 		return LMP_ESPERA;
// 	}else{
// 		read_reg_lmp(GPIO_DAT,&registro,1);
// 		while(state_lmp()!=LMP_OK);
// 		if((bit&registro)==bit){
// 			*value=TRUE;
// 		}else{
// 			*value=FALSE;			
// 		}
// 		return LMP_OK;
// 	}
// }


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
st_lmp power_lmp(uint8_t mode){

	if(write_reg_lmp(POWERCN,&mode,1)){
		printf("Error en la ejecución de escritura para el método powe_lmp.\n");
		return LMP_ERROR;
	}
	return LMP_OK;
}

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
st_lmp config_DRYB_lmp(void){
	uint8_t msg=0x83;

	if(write_reg_lmp(SPI_DRDYBCN,&msg,1)){
		printf("Error en la escritura en registro para configuraciín de DRYB.\n");
	}
	return LMP_OK;
}


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
st_lmp read_ADC_lmp(lmp_dev* dev,uint32_t* lectura){
	uint8_t addr=0X1A;
	uint8_t* data=malloc(4*sizeof(uint8_t));
	read_reg_lmp(dev,addr,data+1,3);
	if((data[1]&0x80)){
		data[0]=0xFF;
	}else{
		data[0]=0x00;
	}
	
	*lectura=array_to_i32(data,4,0);
}

st_lmp parse_byteSize(uint8_t num, uint8_t* sz){
	switch(num){
		case 1:
			*sz=SZ1;
			break;
		case 2:
			*sz=SZ2;
			break;
		case 3:
			*sz=SZ3;
			break;
		default:
			*sz=SZS;
			break;
	}

	return LMP_OK;
}