#include <stdint.h>                                                          
#include <stdlib.h>                                                          
#include <getopt.h>                                                          
#include <linux/types.h>                                                      
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>

size_t size;
char* line;


main()
{
  size=40;
  line=calloc(1,size);

  freopen("/dev/ttyS0","r",stdin);

  while(1){
    printf("Dentro del loop\n");
    getline(&line,&size,stdin);
    printf("Char: %s\n",line);
  }
}

