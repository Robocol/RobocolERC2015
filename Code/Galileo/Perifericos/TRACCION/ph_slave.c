 
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

const uint8_t STEP_SIZE = 0x0A;
const uint8_t DEFAULT_PWM = 30;

const uint8_t FORWARD = 0x01;
const uint8_t BACKWARD = 0x02;
const uint8_t STEER = 0x04;
const uint8_t STOPPED= 0x08;

/*------------------------ PUENTE H ------------------------*/

uint8_t addr=0b0111000;
ph_dev* devptr1;
ph_dev* devptr2;
int salida;
uint8_t state=0;

void parser(char dato, char arg){

	//Setters
	if(dato=='m'){

		printf("Cambiando PWM a: %d \n",arg);
		if((uint8_t)arg<100){
			ph_setPWMSmooth(devptr1,arg,STEP_SIZE);
			ph_setPWMSmooth(devptr2,arg,STEP_SIZE);
		}else{
			ph_setPWMSmooth(devptr1,100,STEP_SIZE);
			ph_setPWMSmooth(devptr2,100,STEP_SIZE);
		}

	}else if(dato=='w'){

		printf("Moviendo hacia adelante\n");
		if(state&BACKWARD){
			ph_setPWMSmooth(devptr1,0,STEP_SIZE);
			ph_setPWMSmooth(devptr2,0,STEP_SIZE);

			ph_setDireccion(devptr1,1);
			ph_setDireccion(devptr2,1);

			ph_setPWMSmooth(devptr1,DEFAULT_PWM,STEP_SIZE);
			ph_setPWMSmooth(devptr2,DEFAULT_PWM,STEP_SIZE);


		}else if((state&STEER)||(state==0)){

			ph_setDireccion(devptr1,1);
			ph_setDireccion(devptr2,1);

 			ph_setPWMSmooth(devptr1,DEFAULT_PWM,STEP_SIZE);
			ph_setPWMSmooth(devptr2,DEFAULT_PWM,STEP_SIZE);

		}

		state=FORWARD;


	}else if(dato=='s'){
		printf("Moviendo hacia atrás\n");
		if(state&FORWARD){
			ph_setPWMSmooth(devptr1,0,STEP_SIZE);
			ph_setPWMSmooth(devptr2,0,STEP_SIZE);

			ph_setDireccion(devptr1,0);
			ph_setDireccion(devptr2,0);

			ph_setPWMSmooth(devptr1,DEFAULT_PWM,STEP_SIZE);
			ph_setPWMSmooth(devptr2,DEFAULT_PWM,STEP_SIZE);
		}else if((state&STEER)||(state==0)){
			ph_setDireccion(devptr1,0);
			ph_setDireccion(devptr2,0);

 			ph_setPWMSmooth(devptr1,DEFAULT_PWM,STEP_SIZE);
			ph_setPWMSmooth(devptr2,DEFAULT_PWM,STEP_SIZE);
		}
		state=BACKWARD;
	}else if(dato=='a'){
		printf("Girando hacia izquierda\n");

		state|=STEER;


	}else if(dato=='d'){
		printf("Girando hacia derecha\n");
		state|=STEER;
		ph_setPWMSmooth(devptr1,(*devptr1).pwm+10,STEP_SIZE);
		ph_setPWMSmooth(devptr2,(*devptr2).pwm+10,STEP_SIZE);


	}else if(dato=='x'){
		printf("PWMs a 0\n");
		state=STOPPED;
		ph_setPWMSmooth(devptr1,0,STEP_SIZE);
		ph_setPWMSmooth(devptr2,0,STEP_SIZE);


	}else if(dato=='g'){
		printf("Acelerando\n");
		if((*devptr1).pwm<100){
			ph_setPWMSmooth(devptr1,(*devptr1).pwm+10,STEP_SIZE);
			ph_setPWMSmooth(devptr2,(*devptr2).pwm+10,STEP_SIZE);
		}else{
			ph_setPWMSmooth(devptr1,100,STEP_SIZE);
			ph_setPWMSmooth(devptr2,100,STEP_SIZE);			
		}


	}else if(dato=='f'){
		printf("Desacelerando\n");
		if((*devptr1).pwm>STEP_SIZE){
			ph_setPWMSmooth(devptr1,(*devptr1).pwm-10,STEP_SIZE);
			ph_setPWMSmooth(devptr2,(*devptr2).pwm-10,STEP_SIZE);
		}else{
			ph_setPWMSmooth(devptr1,0,STEP_SIZE);
			ph_setPWMSmooth(devptr2,0,STEP_SIZE);			
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
		ph_setEstado(devptr1,32);
		ph_setEstado(devptr2,32);
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

	ph_build(&dev1);
	ph_build(&dev2);

	devptr1=&dev1;
	devptr2=&dev2;

	salida=0;


	ph_setEstado(devptr1,32);
	ph_setEstado(devptr2,32);

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


