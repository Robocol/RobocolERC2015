#include "lmp_robocol.h"

st_lmp build_lmp(lmp_dev* dev){

	spi_device* new_spi=malloc(sizeof(spi_device));
	uint8_t addr=0b0111000;

	if(spi_create_device(new_spi,0,(*dev).pin_cs)){
		printf("Error en la creación de dispositivo spi para manejo de puente H.(ph_robocol.c)\n");
		return LMP_ERROR;
	}
	(*dev).spi=new_spi;

	if(spi_start("/dev/spidev1.0",100000)){
		printf("Error en la inicialización interfaz spi.\n");
		return LMP_ERROR;
	}

	if(spi_create_device( (*dev).spi,0,(*dev).pin_cs)){
		printf("Error en la creación del dispositivo spi para comunicación con LMP \n");
		return LMP_OK;
	}

	(*dev).address=0x00;

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
st_lmp write_reg_lmp(lmp_dev* dev, uint8_t address, uint8_t* data, uint8_t num){
	printf("WRITING\n");
	uint8_t* tx_up=malloc(2*sizeof(uint8_t));
	uint8_t* rx_up=malloc(2*sizeof(uint8_t));
	uint8_t* tx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t* rx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t bytes;
	uint8_t iterator;

	parse_byteSize(num,&bytes);

	if (((*dev).address>>4)!=(address>>4)){
		//UPPER ADDRESS
		*(tx_up+1)=(FILLER_T1<<3)|(address>>4);
		*tx_up=WRITE_ADDR;
	//	printf("UPPER ADDRESS:%x%x\n",*tx_up,*(tx_up+1));
		if(spi_rw((*dev).spi,tx_up,rx_up,2)){
			printf("Error en set de upper Addresscon valor: \tAddr:%x\n",address>>4);
			return LMP_ERROR;
		}
		(*dev).address=address;
	}
	//LOWER ADDRES + DATA
	*tx_low=(WRITE<<(sizeof(uint8_t)-1))|(bytes<<(5))|(FILLER_T1<<4)|((address&LOWADDR_SL));
	printf("LOWER TRANS: %x",*tx_low );
	for (iterator= 1; iterator < num+1; ++iterator){
		*(tx_low+iterator)=*(data+iterator-1);
		printf("%x",*(tx_low+iterator));
	}

	printf("\n");

	if(spi_rw((*dev).spi,tx_low,rx_low,num+1)){
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
	printf("READING:\n");
	uint8_t* tx_up=malloc(2*sizeof(uint8_t));
	uint8_t* rx_up=malloc(2*sizeof(uint8_t));
	uint8_t* tx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t* rx_low=malloc((num+1)*sizeof(uint8_t));
	uint8_t nshift=num*sizeof(uint8_t);
	uint8_t bytes;
	uint8_t iterator;



	parse_byteSize(num,&bytes);

	if (((*dev).address>>4)!=(address>>4)){
		//UPPER ADDRESS
		*(tx_up+1)=(FILLER_T1<<3)|(address>>4);
		*tx_up=WRITE_ADDR;
		printf("UPPER ADDRESS:%x%x\n",*tx_up,*(tx_up+1));
		if(spi_rw((*dev).spi,tx_up,rx_up,2)){
			printf("Error en set de upper Addresscon valor: \tAddr:%x\n",address>>4);
			return LMP_ERROR;
		}
		(*dev).address=address;
	}
	//LOWER ADDRESS + DATA
	*tx_low=(READ<<(7))|(bytes<<(5))|(FILLER_T1<<4)|((address&LOWADDR_SL));
	printf("LOWER TRANS: %x\n",*tx_low );

	if(spi_rw((*dev).spi,tx_low,rx_low,num+1)){
		printf("Error en transacción a registro especificado con lower address pasado por parámetro\n"
			"\tAddr:%x\n\tData:%x",LOWADDR_SL&address,*data);
		return LMP_ERROR;
	}

	for (iterator = 0; iterator < num+2; ++iterator){
		printf("Data:%x\n",*(data+iterator) );
	}

	data=rx_low;
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
	uint8_t* data=malloc(4*sizeof(uint8_t));

	if(read_reg_lmp(dev,0x1A,data+1,3)){
		printf("Error en lectura de registro desde read_ADC_lmp\n");
		return LMP_ERROR;
	}
	if((data[1]&0x80)){
		data[0]=0xFF;
	}else{
		data[0]=0x00;
	}
	
	*lectura=array_to_i32(data,4,0);

	return LMP_OK;
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