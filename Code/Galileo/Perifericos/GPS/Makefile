################################################################################################
#	Definición de reglas de compilación de ejecutables con SPI								   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas ara el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../UART \
       -I../../MISC

#Objetos de los cuales depende la compilación de los ejecutables SPI
_OBJ = terminal_robocol$(LEXT)\
       misc_robocol$(LEXT)\
       gps_robocol$(LEXT)

#Concatenación de objetos con ruta de ubicación predeterminada
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

################################################################################################
#	Definición de reglas 																	   #
################################################################################################

#Regla de compilación de ejecutable para prueba del driver
prueba_gps: $(OBJ)
	$(CC) -o $@$(EEXT) prueba_gps.c $^ $(CFLAGS) -lpthread



include ../Makefile

