#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int client_socktfd;
	int client_length;
	int outc_connect;
	struct sockaddr_in client_address;
	char buff[32];

	client_socktfd=socket(AF_INET,SOCK_STREAM,0);
	client_address.sin_family=AF_INET;
	client_address.sin_addr.s_addr=inet_addr("127.0.0.1");
	client_address.sin_port=htons(9876);
	printf("%s\n", "Port Directioned..." );
	client_length=sizeof(client_address);

	printf("%s\n", "Triying Connection..." );
	outc_connect = connect(client_socktfd, (struct sockaddr *)&client_address, client_length);


	if(outc_connect == -1) {
		perror("Oops: Couldnt stablish connection");
		exit(1);
	}
	printf("%s\n", "Successful Connection" );
	printf("%s\n", "Waiting for entry..." );

	while(1){
	scanf("%s",&buff[0]);
	write(client_socktfd, &buff, 32);
	read(client_socktfd, &buff, 32);
	printf("%s\n", buff);
	}

	close(client_socktfd);
	exit(0);

}