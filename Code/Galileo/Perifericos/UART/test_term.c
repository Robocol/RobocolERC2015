#include "terminal_robocol.h"

int main(int argc, char const *argv[])
{
	
	char *menu[] = {
		"w - Just Write",
		"r - Just Read",
		"b - Both of the previous",
		"q - quit",
		NULL,
	};

	term_device* tdev=malloc(sizeof(term_device));
	char* uline= NULL;
	char* buffer=NULL;
	size_t len=0;

	if(term_open(tdev,"/dev/ttyS0","/dev/ttyS0")){
		printf("Error en term_open.\n");
		return 1;
	}



	while(1){

		term_readln(tdev,&buffer);
		printf("%s\n", buffer );
	}

	free(uline);
	free(buffer);

	return 0;
}


uint8_t getchoice(char *greet, char *choices[])
{
	uint8_t chosen = 0;
	uint8_t selected;
	char **option;
	
	printf("Choice: %s\n",greet);
	option = choices;

	do {
		while(*option) {
			printf("%s\n",*option);
			option++;
		}

		selected = getchar();
		option = choices;

		while(*option){
			if(selected == *option[0]) {
				chosen = 1;
				break;
			}
			option++;
		}

		if(!chosen) {
			printf("Incorrect choice, select again\n");
		}

	} while(!chosen);

	return selected;
}
