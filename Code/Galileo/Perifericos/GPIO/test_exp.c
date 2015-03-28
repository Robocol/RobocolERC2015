#include "gpio_robocol.h"

int main(int argc, char const *argv[])
{
	uint8_t fnum;
	uint8_t port;
	char* farg=NULL;
	char* buff;
	if((argc < 2)||(argc>3)){
		fprintf(stderr, "Uso: %s <Function Number> [<Port>]\n",argv[0]);
		exit(1);
	}else{
		fnum=(uint8_t)atoi(argv[1]);
		if (argc==3){
			farg=(char*)argv[2];
		}
		uint8_t addr=0b0111000;
		build_expander(addr);
	}

	switch(fnum){
		case 0:
			if (farg==NULL){
				fprintf(stderr, "Uso: %s gpio_clear <PORT>. Not valid Port\n",argv[0]);
				exit(1);
			}
			port=(uint8_t)atoi(farg);
			gpio_exp_clear(port);
			break;
		case 1:
			if (farg==NULL){
				fprintf(stderr, "Uso: %s gpio_set <PORT>. Not valid Port\n",argv[0]);
				exit(1);
			}
			port=(uint8_t)atoi(farg);
			gpio_exp_set(port);
			break;
		case 2:
			if (farg==NULL){
				fprintf(stderr, "Uso: %s gpio_get <PORT>. Not valid PORT\n",argv[0]);
				exit(1);
			}
			port=(uint8_t)atoi(farg);
			gpio_exp_get(port,buff);
			printf("GPIO VALUE: %d\n",*buff);
			break;
		case 3:
			reg_clear();
			break;
		case 4:
			reg_set();
			break;
		case 5:
			if (farg==NULL){
				fprintf(stderr, "Uso: %s reg_set_value <VALUE>. Not valid VALUE\n",argv[0]);
				exit(1);
			}
			reg_set_value(farg);
			break;
		case 6:
			reg_get(buff);
			printf("REG VALUE: %c\n",*buff);
			break;
		default:
			break;
	}
	close_expander();
	return 0;
}