#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#define PORT_NUM "9876"
#define BUFFSIZE 32
#define MAX_CONNECTIONS 5
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)


void closeWithError(char *error){
	perror(error);
	exit(EXIT_FAILURE);
}

int main()
{
	int server_socktfd, client_socktfd;
	int server_length, client_length;
	struct sockaddr_in client_address;
	char caracter[BUFFSIZE];

	struct addrinfo available;
	struct addrinfo *availablePorts,*aprp;
	int optval;
	char addrStr[ADDRSTRLEN];
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	const char *readError = "read() error , keep going!!!";


	int minutos=10;

	memset(&available,0,sizeof(available));

	available.ai_canonname=NULL;
	available.ai_addr=NULL;
	available.ai_next=NULL;
	available.ai_socktype=SOCK_STREAM;
	available.ai_family=AF_INET;
	available.ai_flags=AI_PASSIVE | AI_NUMERICSERV;

	if(getaddrinfo(NULL,PORT_NUM,&available, &availablePorts)<0)
	{
		closeWithError("getaddrinfo() failed");
	}

	optval=1;
	for(aprp=availablePorts;aprp!=NULL;aprp=aprp->ai_next)
		{	if((server_socktfd=socket(aprp->ai_family, aprp->ai_socktype, aprp->ai_protocol))<0)
			{
				continue;
			}

			if (setsockopt(server_socktfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))<0)
			{
				closeWithError("setsockopt() failed");
			}

			if(bind(server_socktfd,aprp->ai_addr,aprp->ai_addrlen)==0)
			{
				break;
			}

			close(server_socktfd);
		}

		if(aprp==NULL)
		{
			printf("%s\n","No se pudo conectar a ningún puerto" );
			exit(EXIT_FAILURE);
		}

		if(listen(server_socktfd,MAX_CONNECTIONS)<0)
		{
			closeWithError("listen() failed");
		}

		freeaddrinfo(availablePorts);

		printf("%s\n","Server initialized and waiting..." );


		while(1)
		{

			if((client_socktfd= accept(server_socktfd,(struct sockaddr *)&client_address,&client_length))==-1){
				perror("accept() failed");

			}
			client_length=sizeof(client_address);
			printf("%s\n","New Connection Created" );

			if (getnameinfo((struct sockaddr *) &client_address, client_length, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
				snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
			else
				snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
			printf("Connection from %s\n", addrStr);


			while(1){

				if(read(client_socktfd,&caracter,BUFFSIZE)<0){
					perror("read() failed");
					strcpy(caracter,readError);
				}
				if(write(client_socktfd,&caracter,BUFFSIZE)<0){
					close(client_socktfd);
					closeWithError("write() failed");
				}
			}

			if(close(client_socktfd)){
				perror("close() failed");
				continue;
			}
		}


		if(client_socktfd==-1){
			closeWithError("accept() failed");
		}

		return 0;
	}






