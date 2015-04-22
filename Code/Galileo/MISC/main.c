#include "misc_robocol.h"
int main(int argc, char const *argv[])
{
	char* buffer=malloc(sizeof(char));
	if(g_read_file("/sys/class/gpio/gpio37/value",buffer,1)){
		printf("Error en lectura\n");
		return 1;
	}
	printf("%c\n",*buffer );
	return 0;
}