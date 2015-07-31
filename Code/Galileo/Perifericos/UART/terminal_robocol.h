#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>


#ifndef TERM_ERROR
#define TERM_ERROR 1
#endif

#ifndef TERM_OK
#define TERM_OK 0
#endif
typedef uint8_t term_st;

typedef struct term_device{
	FILE* input;
	FILE* output;
}term_device;

term_st term_open(term_device* dev, char* route_in, char* route_out);
term_st term_write(term_device* dev, char* word);
term_st term_readln(term_device* dev, char** buffer);
term_st term_readChar(term_device* dev, char* buffer);
