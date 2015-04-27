################################################################################################
#	Definición de reglas de compilación de ejecutables con PH   							   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas ara el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../I2C \
       -I../GPIO\
       -I../SPI \
       -I../../MISC\
       -I../PH

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
ph_slave: $(OBJ)
	$(CC) -o $@$(EEXT) ph_slave.c $^ $(CFLAGS)

#Regla de compilación para creación de logs
ph_master: $(OBJ)
	$(CC) -o $@$(EEXT) ph_master.c $^ $(CFLAGS)

include ../Makefile