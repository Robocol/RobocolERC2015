#include "servidor_robocol.h"

const uint8_t timer=5;
const char *failSentence="received, 100";
uint8_t connFail=0;

void* cmd(void* arg);

int main(int argc, char const *argv[])
{
	pthread_t thread;
	uint8_t i;
	void *thread_result;

	logOpen("/tmp/logwsfty.log");

	if (pthread_create(&thread, NULL, cmd, (void *)NULL)) {
		printf("Error en creación de thread\n");
		logMessage("Thread creation failed. (gps_robocol -> gps_start )");
	}

	if(ignoreSigpipe()){
		logMessage("Error abriendo el archivo de log de Wireless Safety");
	}

	while(1){
		if (connFail){
			for (i = 0; i < 25; ++i){
				logMessage("Principal Thread:Simulating connection fail. Waiting in 'read' for a command.%d",i);
				sleep(1);
			}
			logMessage("Principal Thread: Connection Established . Command Received.");
			connFail=0;
		}
	}


	return 0;
}

void *cmd(void* arg){
	FILE *shell;
	uint8_t count=0;
	char buff[512];

	logMessage("Thread Created");


	/*if(becomeDaemon(0)==-1){
		logMessage("\nEl proceso del servidor no pudo volverse un demonio\n");
	}*/

	
	logMessage("Apertura de ping\n");
	while(1){
		if (!connFail)
		{
			logMessage("Trying to ping...");
			if(!(shell = popen("ping -c 3 10.5.5.2", "r"))){
				logMessage("Could not open the shell pipe.");
				break;
			}


			if(fgets(buff, sizeof(buff), shell)==NULL){
				logMessage("Error en la lectura inicial\n");
			}
			logMessage("In program before while:%s\n", buff );

			while(fgets(buff, sizeof(buff), shell)!=NULL){
				if (count<3){
					logMessage("Count:%d-->%s",count,buff );
				}else{
					break;
				}
				count++;
			}

			//count=0 . No connection to red handling
			//strstr(failSentence,buff).Connection to network but not to destination host handling
			logMessage("BEFORE STRSTR:%s",buff);
			if (count==0 || (strstr(buff,"transmitted")!=NULL)){
				logMessage("No count.Apagar perifericos.\n");
				connFail=1;
			}

			count=0;


			if(pclose(shell)<0){
				logMessage("Error en pclose\n");

			}

			sleep(timer);
		}
	}
}




