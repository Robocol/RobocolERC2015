#include "gps_robocol.h"

int main(){

	char *line = malloc(100*sizeof(char));
	gps_build("/home/german/GitHub/RobocolERC2015/Code/Galileo/Perifericos/GPS/gps.log");
	gps_start();
	sleep(10);
	printf("Stopping\n");
	gps_stop();

	struct gps_dev datos;
	gps_getData(&datos);

	printf("%f\n",datos.latitude );

	gps_report(line);
	printf("%s\n", line );

	return 0;	
}