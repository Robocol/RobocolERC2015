#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define BUFFSIZE 32
#define DEFAULT_PORT 5000

void closeWithError(char *error){
	perror(error);
	exit(1);
}

int main(int argc, char *argv[]){

	int client_socktfd;
	struct sockaddr_in client_address;
	unsigned short server_port;
	char *server_address;
	char buff[BUFFSIZE];
	int client_length;

	/*Handling function arguments*/
	if((argc < 2)||(argc>3)){
		fprintf(stderr, "Uso: %s <Server IP> [<Echo Port>]\n",argv[0]);
		exit(1);
	}

	server_address=argv[1];

	if(argc==3){
		server_port=atoi(argv[2]);
	}
	else{	
		server_port=DEFAULT_PORT;
	}
	
	if((client_socktfd=socket(AF_INET,SOCK_STREAM,0))<0){
		closeWithError("socket() fallo");
	}

	memset(&client_address,0,sizeof(client_address));
	client_address.sin_family=AF_INET;
	client_address.sin_addr.s_addr=inet_addr(server_address);
	client_address.sin_port=htons(server_port);
	client_length=sizeof(client_address);
	printf("%s\n", "Triying Connection..." );


	if(connect(client_socktfd, (struct sockaddr *)&client_address, client_length)==-1){
		closeWithError("Connection failed");
	}

	printf("%s\n", "Successful Connection" );
	printf("%s\n", "Waiting for entry..." );

	while(1){
	printf("%s","User:" );
	fgets (buff, BUFFSIZE, stdin);
	write(client_socktfd, &buff, BUFFSIZE);
	read(client_socktfd, &buff, BUFFSIZE);
	printf("Server:%s", buff);
	}

	close(client_socktfd);
	exit(0);

}
