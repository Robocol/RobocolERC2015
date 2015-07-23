################################################################################################
#	Definición de reglas de compilación de ejecutables con SPI								   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas ara el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../UART \
       -I../../MISC

#Objetos de los cuales depende la compilación de los ejecutables SPI
_OBJ = uart_robocol$(LEXT)\
       misc_robocol$(LEXT)

#Concatenación de objetos con ruta de ubicación predeterminada
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

################################################################################################
#	Definición de reglas 																	   #
################################################################################################

#Regla de compilación de ejecutable para prueba del driver
uart_test: $(OBJ)
	$(CC) -o $@$(EEXT) uart_test.c $^ $(CFLAGS) 

include ../Makefile

