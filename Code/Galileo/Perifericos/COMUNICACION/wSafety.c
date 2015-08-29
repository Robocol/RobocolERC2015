#include "servidor_robocol.h"

const uint8_t timer=5;
const char *failSentence="received, 100";

int main(int argc, char const *argv[])
{
	FILE *shell;
	uint8_t count=0;
	char buff[512];

	logOpen("/tmp/logwsfty.log");

	if(ignoreSigpipe()){
		logMessage("Error abriendo el archivo de log de Wireless Safety");
	}

	/*if(becomeDaemon(0)==-1){
		logMessage("\nEl proceso del servidor no pudo volverse un demonio\n");
	}*/

	
	logMessage("Apertura de ping\n");
	while(1){

		if(!(shell = popen("ping -c 3 10.5.5.2", "r"))){
			return 1;
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
		}

		count=0;


		if(pclose(shell)<0){
			logMessage("Error en pclose\n");

		}

		sleep(timer);
	}


	return 0;
}

