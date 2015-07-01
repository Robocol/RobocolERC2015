 
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include "ph_robocol.h"

//Cambien la MAC de la Galileo
//98:4F:EE:00:54:9B
#define DEST_MAC0	0x98
#define DEST_MAC1	0x4F
#define DEST_MAC2	0xEE
#define DEST_MAC3	0x00
#define DEST_MAC4	0x54
#define DEST_MAC5	0x9B
 
#define ETHER_TYPE	0x0800
 
#define DEFAULT_IF	"eth0"
#define BUF_SIZ	1024

const uint8_t INC 			=05;
const uint8_t MAX_VEL		=60;
const uint8_t DEFAULT_VEL 	=15;

const uint8_t FORWARD 		=1;
const uint8_t BACKWARD 		=2;
const uint8_t STEER 		=4;
const uint8_t STOPPED		=8;

/*------------------------ PUENTE H ------------------------*/

uint8_t addr=0b0111000;
ph_dev* devptr1;
ph_dev* devptr2;
int salida;
uint8_t state=0;
volatile uint8_t curr_vel,new_vel;



void parser(char dato, char arg){

	//Setters
	if(dato=='m'){

		printf("Cambiando PWM a: %d \n",arg);

		if (arg>MAX_VEL){
			printf("La velocidad no puede exceder el valor maximo definido: \n",MAX_VEL);
			ph_setVel(devptr1,MAX_VEL);
			ph_setVel(devptr2,MAX_VEL);
			printf("La velocidad no puede exceder el valor maximo definido: \n",MAX_VEL);
			curr_vel=MAX_VEL;
		}else{
			ph_setVel(devptr1,arg);
			ph_setVel(devptr2,arg);
			curr_vel=arg;
		}

	}else if(dato=='w'){

		printf("Moviendo hacia adelante\n");
		if(state&BACKWARD){
			ph_setVel(devptr1,0);
			ph_setVel(devptr2,0);

			sleep(2);
			ph_setDireccion(devptr1,1);
			ph_setDireccion(devptr2,1);

			curr_vel=0;


		}else if((state&STEER)||(state==0)){

			ph_setDireccion(devptr1,1);
			ph_setDireccion(devptr2,1);

 			ph_setVel(devptr1,DEFAULT_VEL);
			ph_setVel(devptr2,DEFAULT_VEL);
			curr_vel=DEFAULT_VEL;
		}
		
		state=FORWARD;


	}else if(dato=='s'){
		printf("Moviendo hacia atrás\n");
		if(state&FORWARD){
			ph_setVel(devptr1,0);
			ph_setVel(devptr2,0);

			ph_setDireccion(devptr1,0);
			ph_setDireccion(devptr2,0);

			ph_setVel(devptr1,DEFAULT_VEL);
			ph_setVel(devptr2,DEFAULT_VEL);
		}else if((state&STEER)||(state==0)){
			ph_setDireccion(devptr1,0);
			ph_setDireccion(devptr2,0);

 			ph_setVel(devptr1,DEFAULT_VEL);
			ph_setVel(devptr2,DEFAULT_VEL);
		}
		curr_vel=DEFAULT_VEL;
		state=BACKWARD;
	}else if(dato=='a'){
		printf("Girando hacia izquierda\n");
		new_vel=curr_vel+INC;
		if(state & BACKWARD){
			ph_setDireccion(devptr1,1);
			ph_setDireccion(devptr2,1);
		}else{
			ph_setDireccion(devptr1,0);
			ph_setDireccion(devptr2,0);
		}

		if(state & STEER){
			if (new_vel>MAX_VEL){
				ph_setDireccion(devptr1,0);
				ph_setDireccion(devptr2,0);
				ph_setVel(devptr1,MAX_VEL);
				ph_setVel(devptr2,MAX_VEL);
				printf("Ha alcanzado la mínima vel(0) admisible para los puentesH de Galileo 1\n");
				curr_vel=MAX_VEL;
			}else{
				ph_setDireccion(devptr1,0);
				ph_setDireccion(devptr2,0);
				ph_setVel(devptr1,new_vel);
				ph_setVel(devptr2,new_vel);
				curr_vel=new_vel;
			}
		}else{
				ph_setDireccion(devptr1,0);
				ph_setDireccion(devptr2,0);
				ph_setVel(devptr1,0);
				ph_setVel(devptr2,0);
				curr_vel=0;
		}
		state|=STEER;
	}else if(dato=='d'){
		printf("Girando hacia derecha\n");
		state|=STEER;
		new_vel=curr_vel+INC;
		if (new_vel>MAX_VEL){
			ph_setVel(devptr1,MAX_VEL);
			ph_setVel(devptr2,MAX_VEL);
			printf("Ha alcanzado el maximo vel admisible para los puentesH de Galileo 1\n");
			curr_vel=MAX_VEL;
		}else{
			ph_setVel(devptr1,new_vel);
			ph_setVel(devptr2,new_vel);
			curr_vel=new_vel;
		}
	}else if(dato=='x'){
		printf("PWMs a 0\n");
		state=STOPPED;
		ph_setVel(devptr1,0);
		ph_setVel(devptr2,0);
	}else if(dato=='g'){
		printf("Acelerando\n");
		new_vel=curr_vel+INC;
		if (new_vel>MAX_VEL){
			ph_setVel(devptr1,MAX_VEL);
			ph_setVel(devptr2,MAX_VEL);
			printf("Ha alcanzado el maximo vel admisible para los puentesH de Galileo 1\n");
			curr_vel=MAX_VEL;
		}else{
			ph_setVel(devptr1,new_vel);
			ph_setVel(devptr2,new_vel);
			curr_vel=new_vel;
		};
	}else if(dato=='f'){
		printf("Desacelerando\n");
		new_vel=curr_vel-INC;
		if (new_vel<0){
			ph_setVel(devptr1,0);
			ph_setVel(devptr2,0);
			printf("Ha alcanzado el maximo vel admisible para los puentesH de Galileo 1\n");
			curr_vel=0;
		}else{
			ph_setVel(devptr1,new_vel);
			ph_setVel(devptr2,new_vel);
			curr_vel=new_vel;
		}	
	}else if(dato=='e'){
		printf("Motores activados\n");
		ph_enable(devptr1);
		ph_enable(devptr2);

	}else if(dato=='p'){
		printf("Motores desactivados\n");
		ph_disable(devptr1);
		ph_disable(devptr2);
	}else if(dato=='q'){
		printf("Cerrando el programa. Adiós\n");
		salida=1;
	}else if(dato=='c'){
		printf("Cambiando estado\n");
		ph_setEstado(devptr1,16);
		ph_setEstado(devptr2,16);
	}else if(dato=='n'){
		ph_setEstado(devptr1,32);
		ph_setEstado(devptr2,32);
		ph_setPWM(devptr1,0);
		ph_setPWM(devptr2,0);

		printf("FRENO DE EMERGENCIA\n");

	}else{
		printf("Comando desconocido\n");
	}
	
}





int main(int argc, char *argv[]){

	/*------------------------ PUENTE H ------------------------*/
	struct sockaddr_ll sock_address;
	char sender[INET6_ADDRSTRLEN];
	int sockfd, ret, i;
	int sockopt;
	ssize_t numbytes;
	struct ifreq ifopts; /* Modo Tavera, modo promiscuo o puta */
	struct ifreq if_ip;  /* Estructura para guardar la IP por si acaso */
	struct sockaddr_storage their_addr;
	uint8_t buf[BUF_SIZ];
	char ifName[IFNAMSIZ];


	printf("----------Prueba Movimiento----------\n");
	
	ph_dev dev1={PINA0,0,1,4,0};
	ph_dev dev2={PINA1,2,3,4,0};
	dev1.addr=EXP1;
	dev2.addr=EXP1;
	ph_build(&dev1);
	ph_build(&dev2);

	devptr1=&dev1;
	devptr2=&dev2;

	salida=0;

	if(ph_setEstado(devptr1,16)){
	printf("Error en set de Estado a 16(prueba_ph.c)\n");
	perror("Descripción");		
	}
	if(ph_setVel(devptr1,0)){
		printf("Error en set de velocidad(prueba_ph.c)\n");
		perror("Descripción");	
	}
	if(ph_setEstado(devptr2,16)){
		printf("Error en set de Estado a 16(prueba_ph.c)\n");
		perror("Descripción");		
	}
	if(ph_setVel(devptr2,0)){
		printf("Error en set de PWM(prueba_ph.c)\n");
		perror("Descripción");	
	}

	/* Se copia la interfaz de red */

	strcpy(ifName, DEFAULT_IF);
	 
	/* Estructuras de encabezados */
	struct ether_header *eh = (struct ether_header *) buf;
	struct iphdr *iph = (struct iphdr *) (buf + sizeof(struct ether_header));
	struct udphdr *udph = (struct udphdr *) (buf + sizeof(struct iphdr) + sizeof(struct ether_header));
	 
	memset(&if_ip, 0, sizeof(struct ifreq));
	 
	/* Apertura del socket */
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
	perror("listener: socket");
	return -1;
	}
	 
	/* Modo promiscuo */
	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
	/* Allow the socket to be reused - incase connection is closed prematurely */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
	perror("setsockopt");
	close(sockfd);
	exit(EXIT_FAILURE);
	}
	/* Bind to device */

	memset(&sock_address, 0, sizeof(sock_address));
	sock_address.sll_family = PF_PACKET;
	sock_address.sll_protocol = htons(ETH_P_ALL);
	sock_address.sll_ifindex = if_nametoindex(ifName);
	if (bind(sockfd, (struct sockaddr*) &sock_address, sizeof(sock_address)) < 0) {
	  perror("bind failed\n");
	  close(sockfd);
	  return -4;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1) {
	perror("SO_BINDTODEVICE");
	close(sockfd);
	exit(EXIT_FAILURE);
	}


	 while(!salida){
		printf("listener: Waiting to recvfrom...\n");
		numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0, NULL, NULL);
		printf("listener: got packet %lu bytes\n", numbytes);
		 
		printf("%d -%d-%d-%d-%d-%d\n", eh->ether_dhost[0] == DEST_MAC0,
			eh->ether_dhost[1] == DEST_MAC1 ,
			eh->ether_dhost[2] == DEST_MAC2 ,
			eh->ether_dhost[3] == DEST_MAC3 ,
			eh->ether_dhost[4] == DEST_MAC4 ,
			eh->ether_dhost[5] == DEST_MAC5);

		/* Verifica la MAC */
			if (eh->ether_dhost[0] == DEST_MAC0 &&
			eh->ether_dhost[1] == DEST_MAC1 &&
			eh->ether_dhost[2] == DEST_MAC2 &&
			eh->ether_dhost[3] == DEST_MAC3 &&
			eh->ether_dhost[4] == DEST_MAC4 &&
			eh->ether_dhost[5] == DEST_MAC5) {
				printf("Correct destination MAC address\n");

				/*Se reconoce la instrucción*/

				printf("\tData:");
				for (i=0; i<numbytes; i++) {
					printf("%x:", buf[i]);
			}

			printf("\n %x:%x\n",buf[12],buf[13]);
			parser(buf[14],buf[15]);

		} else {
			printf("Wrong destination MAC: %x:%x:%x:%x:%x:%x\n",
			eh->ether_dhost[0],
			eh->ether_dhost[1],
			eh->ether_dhost[2],
			eh->ether_dhost[3],
			eh->ether_dhost[4],
			eh->ether_dhost[5]
			);
			ret = -1;
		}
	 }

	close(sockfd);
	return ret;
}


