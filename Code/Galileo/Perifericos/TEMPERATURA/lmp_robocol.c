#include "lmp_robocol.h"

st_lmp build_lmp(lmp_dev* dev){
	uint8_t* tx=malloc(2*sizeof(uint8_t));
	uint8_t* rx=malloc(2*sizeof(uint8_t));


	spi_device* new_spi=malloc(sizeof(spi_device));

	printf("Iniciando creación de dispositivo spi para manejo de puenteH\n");
	if(spi_create_device(new_spi,0,(*dev).pin_cs)){
		printf("Error en la creación de dispositivo spi para manejo de puente H.(ph_robocol.c)\n");
		return LMP_ERROR;
	}
	(*dev).spi=new_spi;
	if(spi_start("/dev/spidev1.0",100000)){
		printf("Error en el arranque comunicación spi para manejo de puente H.(ph_robocol.c)\n");
		return LMP_ERROR;
	}

	return LMP_OK;

}


// /*
// ** ===================================================================
// **     Método      :  write_reg_lmp
// */
// /*!
// **     @resumen
// **         Escribe cierta cantidad de datos en un registro seleccio-
// **         nado. Puede escribir más de un dato de forma secuencial lo
// **         cual permite escribir registros aledaños.
// **     @param
// **         address     	   - Dirección del registro
// **     @param
// **         data     	   - Puntero a los datos que se van a escribir
// **     @param
// **         num	     	   - Número de bytes a escribir.
// **     @return
// **                         - Estado de la comuniación antes de enviar. 
// */
// /* ===================================================================*/
// st_lmp write_reg_lmp(lmp_dev* dev, uint8_t address,uint8_t* data){

// 	uint8_t* tx=malloc(2*sizeof(uint8_t));
// 	uint8_t* rx=malloc(2*sizeof(uint8_t));
// 	tx[0]=0x10;// Write To Register Instruction
// 	tx[1]=0x03&(address>>4);
// 	if(spi_rw(dev,tx,rx,2)){
// 		printf("Error en set de upper Address para Configuración de Background Calibration\n");
// 		return LMP_ERROR;
// 	}
			
// 	tx[0]=(0x0F&address);
// 	tx[1]=0x00;
// 	if(spi_rw(dev,tx,rx,2)){
// 		printf("Error en transacción a registro especificado con lower address pasado por parámetro\n"
// 			"\tAddr:%d\n\tData:%d",0x0F&address,*data);
// 		return LMP_ERROR;
// 	}



// 	return LMP_OK;
// }

// /*
// ** ===================================================================
// **     Método      :  read_reg_lmp
// */
// /*!
// **     @resumen
// **         Lee cierta cantidad de datos  un registro seleccionado. 
// **         Puede leer más de un dato lo cual permite leer registros 
// **         aledaños. 
// **     @param
// **         address     	   - Dirección del registro
// **     @param
// **         data     	   - Puntero a la variable donde se van a 
// **         					 escribir los datos.
// **     @param
// **         num	     	   - Número de bytes a leer
// **     @return
// **                         - Estado de la comuniación antes de enviar. 
// */
// /* ===================================================================*/
// st_lmp read_reg_lmp(lmp_dev* dev,uint8_t address,uint8_t* data){

// 	uint8_t* tx=malloc(2*sizeof(uint8_t));
// 	uint8_t* rx=malloc(2*sizeof(uint8_t));
// 	tx[0]=0x10;// Write To Register Instruction
// 	tx[1]=0x03&(address>>4);
// 	if(spi_rw(dev,tx,rx,2)){
// 		printf("Error en set de upper Address para Configuración de Background Calibration\n");
// 		return LMP_ERROR;
// 	}
			
// 	tx[0]=(0x08<<7)|(0x0F&address);
// 	tx[1]=0x00;
// 	if(spi_rw(dev,tx,rx,2)){
// 		printf("Error en transacción a registro especificado con lower address pasado por parámetro\n"
// 			"\tAddr:%d\n\tData:%d",0x0F&address,*data);
// 		return LMP_ERROR;
// 	}
// 	data=rx;
// 	return LMP_OK;
// }

// // /*
// // ** ===================================================================
// // **     Método      :  escribir_io_lmp
// // */
// // /*!
// // **     @resumen
// // **         Escribe el value deseado en los puertos digitales. Los bits
// // **         deben estar habilitados como salidas en el registro GPIO_DIRCN
// // **         sino la asignación del value no tendrá efecto.
// // **     @param
// // **         salidas     	   - Se coloca el value del uint8_t en los pines
// // **         					 del puerto paralelo asignados como salida.
// // **     @return
// // **                         - Estado de la comuniación antes de enviar. 
// // */
// // /* ===================================================================*/
// // st_lmp write_io_lmp(uint8_t salidas){
// // 	if(lmp_ocupado){
// // 		return LMP_ESPERA;
// // 	}else{
// // 		write_reg_lmp(GPIO_DAT,&salidas,1);				
// // 		return LMP_OK;
// // 	}
// // }


// // ** ===================================================================
// // **     Método      :  leer_io_lmp

// // /*!
// // **     @resumen
// // **         Lee el value del bit indicado y lo devuelve en la dirección
// // **         indicada.
// // **     @param
// // **         bit	     	   - Indica cual es el bit a leer (D0-D6).
// // **     @param
// // **         value     	   - Puntero que guarda el value del bit en un
// // **         					 un uint8_t (TRUE=1, FALSE=0).
// // **     @return
// // **                         - Estado de la comuniación antes de enviar. 
// // */
// // /* ===================================================================*/
// // st_lmp read_io_lmp(uint8_t bit, uint8_t* value){
// // 	uint8_t registro=NULL;
// // 	if(lmp_ocupado){
// // 		return LMP_ESPERA;
// // 	}else{
// // 		read_reg_lmp(GPIO_DAT,&registro,1);
// // 		while(state_lmp()!=LMP_OK);
// // 		if((bit&registro)==bit){
// // 			*value=TRUE;
// // 		}else{
// // 			*value=FALSE;			
// // 		}
// // 		return LMP_OK;
// // 	}
// // }


// /*
// ** ===================================================================
// **     Método      :  power_lmp
// */
// /*!
// **     @resumen
// **         Cambia el estado de la alimentación del dispositivo.
// **     @param
// **         mode	    	   - Modo de operación deseado (LMP_STBY,LMP_OFF,
// **         					 LMP_ON).
// **     @return
// **                         - Estado de la comuniación antes de enviar. 
// */
// /* ===================================================================*/
// st_lmp power_lmp(uint8_t mode){

// 	if(write_reg_lmp(POWERCN,&mode,1)){
// 		printf("Error en la ejecución de escritura para el método powe_lmp.\n");
// 		return LMP_ERROR;
// 	}
// 	return LMP_OK;
// }

// /*
// ** ===================================================================
// **     Método      :  config_DRYB_lmp
// */
// /*!
// **     @resumen
// **         Configura el pin DRYB para que salga por D6.
// **     @return
// **                         - Estado de la comuniación antes de enviar. 
// */
// /* ===================================================================*/
// st_lmp config_DRYB_lmp(void){
// 	uint8_t msg=0x83;

// 	if(write_reg_lmp(SPI_DRDYBCN,&msg,1)){
// 		printf("Error en la escritura en registro para configuraciín de DRYB.\n");
// 	}
// 	return LMP_OK;
// }


// /*
// ** ===================================================================
// **     Método      :  leer_ADC_lmp
// */
// /*!
// **     @resumen
// **         Lee la conversión del ADC.
// **     @param
// **         lectura    	   - Puntero que almacena la lectura.
// **     @return
// **                         - Estado de la comuniación antes de enviar. 
// */
// /* ===================================================================*/
// st_lmp read_ADC_lmp(float* lectura){
// 	uint8_t* txADC=malloc(4*sizeof(uint8_t));
// 	uint8_t* rxADC=malloc(4*sizeof(uint8_t));
// 	float adc,temperature;
// 	txADC[0]=0xCA;
// 	txADC[1]=0x00;
// 	txADC[2]=0x00;
// 	txADC[3]=0x00;
// 	if(spi_rw(&dev,txADC,rxADC,3)){
// 		printf("Error en transacción para lectura de ADC\n");
// 	}
// 	rxADC[0]=0x00;
// 	adc=((float)(array_to_i32(rxADC,4,0)-1792)*1.905)/(8*8388608);
// 	*lectura=adc;
// }
