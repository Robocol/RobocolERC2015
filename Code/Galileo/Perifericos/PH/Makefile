################################################################################################
#	Definición de reglas de compilación de ejecutables con PH   							   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas ara el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../I2C \
       -I../GPIO\
       -I../SPI \
       -I../../MISC

#Objetos de los cuales depende la compilación de los ejecutables PH
_OBJ = spi_robocol$(LEXT) \
       i2c_robocol$(LEXT) \
       gpio_robocol$(LEXT)\
       ph_robocol$(LEXT) \
       misc_robocol$(LEXT)

#Concatenación de objetos con ruta de ubicación predeterminada
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

################################################################################################
#	Definición de reglas 																	   #
################################################################################################

#Regla de compilación de ejecutable para prueba del driver
prueba_ph: $(OBJ)
	$(CC) -o $@$(EEXT) prueba_ph.c $^ $(CFLAGS)

#Regla de compilación para creación de logs
logvc_ph: $(OBJ)
	$(CC) -o $@$(EEXT) logvc_ph.c $^ $(CFLAGS)

include ../Makefile