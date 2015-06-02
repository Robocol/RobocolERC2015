#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <string.h>
#include "ph_robocol.h"


#define MY_DEST_MAC0	0x98
#define MY_DEST_MAC1	0x4F
#define MY_DEST_MAC2	0xEE
#define MY_DEST_MAC3	0x00
#define MY_DEST_MAC4	0x54
#define MY_DEST_MAC5	0x9B
 
#define DEFAULT_IF	"eth0"
#define BUF_SIZ	1024

const uint8_t STEP_SIZE=10;
size_t size=40;	
char sendbuf[BUF_SIZ];					//Tamaño del buffer para nueavas lineas
char* line;			//Puntero al buffer de linea
uint8_t temp,vel,corr,est,debug;	//Buffers para los diferentes mediciones
uint8_t addr=0b0111000;				//Direccion expansor I2C
ph_dev* devptr1;						//puntero a puente h
ph_dev* devptr2;						//puntero a puente h
int buf;							//buff para envío de mensajes
int tx_len = 0;
int inittx=0;

void parser(void);

int main(int argc, char *argv[])
{
	line==malloc(size);
	int i;
	int sockfd;
	struct ifreq if_idx;
	struct ifreq if_mac;
	struct ether_header *eh = (struct ether_header *) sendbuf;
	struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
	struct sockaddr_ll socket_address;
	char ifName[IFNAMSIZ];

/* Se copia la interfaz de red */

	strcpy(ifName, DEFAULT_IF);

/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
		perror("socket");
	}

/* Indice de la interfaz */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
		perror("SIOCGIFINDEX");
/* Adquirir la MAC del que envía */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
		perror("SIOCGIFHWADDR");

/* Construcción del encabezado de Ethernet capa 2 */
	memset(sendbuf, 0, BUF_SIZ);
/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = MY_DEST_MAC0;
	eh->ether_dhost[1] = MY_DEST_MAC1;
	eh->ether_dhost[2] = MY_DEST_MAC2;
	eh->ether_dhost[3] = MY_DEST_MAC3;
	eh->ether_dhost[4] = MY_DEST_MAC4;
	eh->ether_dhost[5] = MY_DEST_MAC5;
/* Ethertype field */
	eh->ether_type = htons(ETH_P_ALL);
	tx_len += sizeof(struct ether_header);
	inittx=tx_len;
/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
/* Destination MAC */
	socket_address.sll_addr[0] = MY_DEST_MAC0;
	socket_address.sll_addr[1] = MY_DEST_MAC1;
	socket_address.sll_addr[2] = MY_DEST_MAC2;
	socket_address.sll_addr[3] = MY_DEST_MAC3;
	socket_address.sll_addr[4] = MY_DEST_MAC4;
	socket_address.sll_addr[5] = MY_DEST_MAC5;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

	/* Datos a enviar pongan aca lo que hay que enviar de la Galileo que recibe por SSH */
	temp, vel, corr, est, debug=0x5A;	

	ph_dev dev1={PINA0,0,1,4,0};			//Creación de primer puente h con pines ina=0, inb=1, enable=4, pwm=0
	ph_dev dev2={PINA1,2,3,4,0};			//Creación de segundo puente h con pines ina=2, inb=3, enable=4, pwm=0


	//Instrucciones de uso para el usuario
		printf("Bienvenido al test de funcionamiento de Puente H (URC 2015-ROBOCOL).\n Utilice una de los siguientes comandos:\n" 
		"\t enable\t\t\t-Habilita o deshabilita el puente H. Valores de entrada 1 o 0\n"
		"\t pwm\t\t-Cambia ciclo útil del PWM. Valor de entrada entre 0 y 255\n"
		"\t w\t \t-Adelante\n"
		"\t a\t \t-Izquierda\n"
		"\t s\t \t-Atras\n"
		"\t d\t \t-Derecha\n"
		"\t c\t \t-Cambiar estado\n");
while(1){
		tx_len = inittx;
		parser();
		printf("\tData:");
		for (i=0; i<15; i++){
			printf("%02x:", sendbuf[i]);
		}
		tx_len++;
		printf("\n%d\n",tx_len );
		if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0){
			printf("Send failed\n");
			perror("ERROR");
		}
	}
return 0;
} 



void parser(void){
	printf("Ingrese un comando\n");
	getline(&line,&size,stdin);			//Lectura de comando
	printf("El comando ingresado fue: %s \n",line);
	if(!strcmp(line,"w\n")){
		sendbuf[tx_len++]='w';

	}else if(!strcmp(line,"a\n")){
		sendbuf[tx_len++]='a';

	}else if(!strcmp(line,"s\n")){
		sendbuf[tx_len++]='s';

	}else if(!strcmp(line,"d\n")){
		sendbuf[tx_len++]='d';

	}else if(!strcmp(line,"pwm\n")){
		printf("Ingrese el PWM deseado:\n");
		sendbuf[tx_len++] ='m';
	}else if(!strcmp(line,"c\n")){
		sendbuf[tx_len++] ='c';

	}else if(!strcmp(line,"enable\n")){
		printf("Ingrese 1 para activar, 0 para desactivar:\n");
		int i =1;
		getline(&line,&i,stdin);
		buf=atoi(line);
		if(buf){
			sendbuf[tx_len++] ='e';

		}else{
			sendbuf[tx_len++] ='p';

		}
		printf("Cambiando enable a: %d \n",buf);
		
	}else{
	printf("USO:\n" 
		"\t enable\t\t\t-Habilita o deshabilita el puente H. Valores de entrada 1 o 0\n"
		"\t pwm\t\t-Cambia ciclo útil del PWM. Valor de entrada entre 0 y 255\n"
		"\t w\t \t-Adelante\n"
		"\t a\t \t-Izquierda\n"
		"\t s\t \t-Atras\n"
		"\t d\t \t-Derecha\n"
		"\t c\t \t-Cambiar estado\n");	
	}

}