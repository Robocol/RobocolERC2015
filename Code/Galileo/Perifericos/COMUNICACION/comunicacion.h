/*
 * --------------------------------------------------------------------------------------
 * comunicacion.h (0.5)
 *
 *  Created on: Apr 1, 2015
 *
 *      Authors:		Sergio Bacca
 *				Daniel Ochoa 
 *				    Germán Giraldo
 * -------------------------------------------------------------------------------------
 */

#ifndef COM_ROBOCOL_H
#define COM_ROBOCOL_H


/*
** =====================================================================================
**     USO DEL DRIVER
*/
/*!
**     @resumen
**          Esta librería incluye los métodos de alto nivel que manejan las comunicaciones internas del ROVER  y las comunicaciones
**		del ROVER con el cuarto de control. El ROVER tiene 4 modos de operación: 
**
**          mode 			- Modo de inicio (INIT): En este modo las Galileo se encienden, se ejecutan sus scripts de inicio y **					se sincroniza su reloj. Una vez sincronizados los relojs la tarjeta cambia a estado SAFE. En este **					estado se ejecuta el main que utiliza esta librería.
**
**					- Modo de operación segura (SAFE): Es el estado incial del ROVER después del encendido. En 
**					este modo el ROVER inicia todos los drivers y realiza un diagnóstico general de su compor-
**					tamiento. En este modo NO se puede mover ningún actuador o motor pero sí se puede verificar la
**					comunicación con todas las tarjetas. Adicionalmente, en este modo el ROVER está a la expectativa
**					de una orden por cualquiera de las alternativas de comunicación externa que lo haga cambiar a modo
**					normal. Si hay alguna falla en la comunicación el ROVER lo informará a nivel de la interfaz; **					dependiendo de la falla se podrá o no pasar a modo NORMAL. En este modo se pueden hacer los cambios **					de configuración necesarios en cada tarjeta: Negociar los diccionarios
**
**					- Modo de operación normal (NORMAL): En este modo el ROVER cumple con un ciclo de operación que 
**					se encarga de adquirir, compilar y transmitir los diagnósticos. Además actualiza los setpoints de
**					los controladores con base en lo que determine el usuario a nivel de la interfaz de usuario. La **					operación en este modo es cíclica. Es decir, cada cierto tiempo se adquieren los datos de diagnóstico
**					y cada cierto tiempo se actualizan los setpoints.
**
**					- Modo de recuperación de fallas (FAIL): En este modo el ROVER sigue operando ya que la falla que 
**					pudo haberse producido no afecta la operación completa del ROVER. La falla se indica a nivel de la **					interfaz de monitoreo y el operador de dicha interfaz puede tomar acciones de recuperación mientras **					que el operador principal maneja el ROVER. En caso que la falla sea una falla total, el sistema **					ingresa a modo de operación segura y proporciona un diagnóstico. En resumen, en este modo el ROVER 
**					funciona de forma acíclica: no envía diagnósticos ciclicamente ya que permite pequeñas interrupciones
**					en el ciclo para recuperar las fallas parciales.
**
**
*/
/* =====================================================================================*/


/*--------------------------------------------------------------------------*/
/*
 *                 INCLUSIÓN DE LIBRERIAS
 */
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <syslog.h>
#include <net/if.h>
#include <netinet/ether.h>

#include <linux/ip.h>
#include <linux/udp.h>

#include "ph_robocol.h"
#include "stepper.h"
#include "sensores.h"
#include "gps.h"



/* Estado de finalización de los métodos de la librería */
typedef char rover_st;
#define ROVER_OK 		0x00
#define ROVER_ERROR 		0x01

/* Tipos de dispositivos para la comunicación  */
typedef char com_device;
#define ETHERNET 0x00
#define WIFI	0x01
#define	BLUETOOTH 0x02


/* Modos de operación del ROVER */
typedef char rover_mode;
#define INIT 0X00
#define SAFE 0x01
#define NORMAL_OP 0x02
#define FAULT  0x03

/* Modos de operación de cada tarjeta */
typedef char galileo_role;
#define MASTER 0x00
#define SLAVE  0x04

/* Tipos de mensajes */
typedef char message_type;
#define ORDEN_CICLICA 0X01
#define CONFIGURACION 0X02
#define DIAGNOSTICO   0X03
#define FALLA	0X04
#define SYNC	0X05

/* Modos de operacion del brazo */
typedef char arm_mode;
#define MODO_PABLO 0X01
#define MODO_NORMAL 0x02

/* Movemientos predefinidos brazo */
typedef char arm_moves;



/*--------------------------------------------------------------------------*/
/*
*                 ESTRUCTURAS ASOCIADAS A LA INFORMACIÓN DE
*				LOS DISPOSITIVOS
*/
/*--------------------------------------------------------------------------*/

/* Estructura de puente H */
typedef struct{ 
	char modo_motor;
	char direccion_giro;
	uint8_t corriente;
	uint8_t vel_pos;//Si es del brazo es posicion, si es tracción es velocidad
	uint8_t temperatura_PH;	
} info_puente_H;

/* Estructura de variables de control */
typedef struct{ 
	uint16_t Kp_corriente;
	uint16_t Ki_corriente;
	uint16_t Kp_velocidad;
	uint16_t Ki_velocidad;
	uint8_t sp_velocidad;
} info_control_ph_traccion;

/* Parámetros para un perfil de curva S (servocontrol)*/
typedef struct{ 
	uint8_t p2;
	uint8_t p3;		
} info_control_ph_brazo;

/* Parámetros para el perfil del paso */
typedef struct{ 
	uint8_t umbral_corriente;
	uint8_t corriente_ref;
	uint8_t tiempo_apagado;
	uint8_t tiempo_encendido;
} info_control_stepper;

/* Estructura de Motor de Pasos */
typedef struct{ 
	char mode;
	char direccion;
	uint32_t periodo; //nanosegundos
	uint16_t status; //Registro STATUS del L6474
	int  
} info_motor_pasos;

/* Estructura de Sensores */
typedef struct{ 
	uint16_t conductividad;
	uint16_t medicion_ph;
	uint16_t humedad;	
} info_sensores;


/*--------------------------------------------------------------------------*/
/*
*                 	ESTRUCTURAS DE DICCIONARIO
*/
/*--------------------------------------------------------------------------*/
/*
* Son estructuras que contienen toda la información que se va a comunicar. Es
* decir, hay un diccionario por cada tipo de mensaje. En total hay 3 tipos de
* mensaje diferentes. La filosofía utilizada es un mapa de direcciones. En el
* estado de inicio los distintos nodos negocian el diccionario utilizando un 
* SCRIPT. Dependiendo del modo de la tarjeta, se puede enviar cierto tipo de 
* mensajes. En modo de inicio solo se pueden enviar mensajes de sincronización
* de reloj los cuales contienen una estructura de info_hora nada mas.
*/
/*--------------------------------------------------------------------------*/


/* Define el diccionario del diagnóstico que debe llegar a la interfaz */
typedef struct{
	struct timeval tv; //Estructura que contiene el tiempo

	#ifdef TRACCION
	uint16_t balancin;
	info_puente_H puenteH1;
	info_puente_H puenteH2;
	info_control_ph_traccion control_ph;
	#endif

	#ifdef ANTENA
	info_motor_pasos giro_antena;
	info_motor_pasos angulo_antena;
	info_control_stepper control_giro_antena;
	info_control_stepper control_angulo_antena;
	#endif
		
	#ifdef TEMPERATURA
	uint16_t temperatura_motor_1;
	uint16_t temperatura_motor_2;
	#endif

	#ifdef GPS
	char datos_GPS[80];
	#endif
		
	#ifdef SENSORES
	info_sensores sensores;
	#endif

	#ifdef BRAZO
	info_puente_H puenteH_base;
	info_puente_H puenteH_hombro;
	info_puente_H puenteH_brazo;
	info_control_ph_brazo perfil_base;
	info_control_ph_brazo perfil_hombro;
	info_control_ph_brazo perfil_brazo;

	info_motor_pasos giro_munheca;
	info_motor_pasos munheca;	
	info_motor_pasos pinza;
	info_control_stepper control_giro_munheca;
	info_control_stepper control_munheca;
	info_control_stepper control_pinza;
	#endif		
} diccionario_diagnostico;

/* Define las opciones de configuración */
typedef struct{
	struct timeval tv; //Estructura que contiene el tiempo
	#ifdef TRACCION
	info_control_ph_traccion control_ph;
	#endif

	#ifdef ANTENA
	info_control_stepper control_giro_antena;
	info_control_stepper control_angulo_antena;
	#endif
		
	#ifdef SENSORES
	uint8_t ch1;
	uint8_t ch2;
	uint8_t ch3;
	#endif

	#ifdef BRAZO
	info_control_ph_brazo perfil_base;
	info_control_ph_brazo perfil_hombro;
	info_control_ph_brazo perfil_brazo;
	info_control_stepper control_giro_munheca;
	info_control_stepper control_munheca;
	info_control_stepper control_pinza;
	#endif
} diccionario_configuracion;

/* Define el diccionario de órdenes cíclicas */
typedef struct{
	struct timeval tv; //Estructura que contiene el tiempo

	#ifdef TRACCION
	uint8_t setpoint1;
	uint8_t setpoint2;
	#endif

	#ifdef ANTENA
	uint16_t pasos_giro;
	uint16_t pasos_levantamineto;
	#endif
		
	#ifdef BRAZO
	char modo_operacion;
	uint8_t posicion_base;
	uint8_t posicion_brazo;	
	uint8_t posicion_antebrazo;
	uint8_t posicion_giro;
	uint8_t posicion_munheca;
	uint8_t posicion_pinza;
	char posiciones_predeterminadas;
	#endif		

} diccionario_ordenes_ciclicas;

/* Define el diccionario de fallas */
typedef struct{
	struct timeval tv; //Estructura que contiene el tiempo

	#ifdef TRACCION
	char error_puenteH1;
	char error_puenteH2;
	#endif

	#ifdef ANTENA
	info_motor_pasos giro_antena;
	info_motor_pasos angulo_antena;
	#endif
		
	#ifdef TEMPERATURA
	char error_temp_mot1;
	char error_temp_mot2;
	#endif

	#ifdef GPS
	char datos_GPS[80];
	#endif
		
	#ifdef SENSORES
	char errores_sensores;
	#endif

	#ifdef BRAZO
	char error_base;
	char error_brazo;
	char error_antebrazo;
	char error_ 	
	#endif		

} diccionario_fallas;








struct dispositivo{
	uint8_t ID;
	char mac[6];
	uint8_t modo;	
}


/*
** ===================================================================
**     Método      :  become_daemon
*/
/*!
**     @resumen
**          Convierte un proceso en un demonio cuando es llamado. 
**			
**     @param
**          flags     	   	- Banderas de configuración del demonio
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
int becomeDaemon(int flags);

/*
** ===================================================================
**     Método      :  sighupHandler
*/
/*!
**     @resumen
**          Maneja las señales del demonio, en especial la señal SIGHUP. 
**			
**     @param
**          sig     	   	- Vector de señal
*/
/* ===================================================================*/
static void sighupHandler(int sig);


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
rover_st seleccionar_com();


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
rover_st sincronizar_reloj();

/*





rover_st verificar_tarjetas();



rover_st configurar_tarjetas();



rover_st obtener_diagnosticos();

rover_st exportar_diccionario();

rover_st enviar_diagnostico();

rover_st reportar_fallas();

rover_st actualizar_operacion();

rover_st cambiar_estado();

rover_st exportar_log();

rover_st calcular_perfil();

rover_st configurar_socket();

rover_st cerrar_socket();

rover_st resolver_comunicacion();

rover_st control_relacion();

rover_st relacionar_perfiles();

rover_st mover_rover();

rover_st posicionar_brazo();

rover_st leer_GPS();



#endif

