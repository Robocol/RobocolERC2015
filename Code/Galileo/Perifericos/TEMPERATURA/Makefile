################################################################################################
#	Definición de reglas de compilación de ejecutables con SPI								   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas ara el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../I2C \
       -I../GPIO \
       -I../SPI \
       -I../../MISC

#Objetos de los cuales depende la compilación de los ejecutables SPI
_OBJ = spi_robocol$(LEXT) \
       i2c_robocol$(LEXT) \
       gpio_robocol$(LEXT) \
       misc_robocol$(LEXT)\
       lmp_robocol$(LEXT)

#Concatenación de objetos con ruta de ubicación predeterminada
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

################################################################################################
#	Definición de reglas 																	   #
################################################################################################
all: afe_multi_in lmp_cafe

#Regla de compilación de ejecutable para prueba del driver
afe_multi_in: $(OBJ)
	$(CC) -o $@$(EEXT) $@.c $^ $(CFLAGS)

#Regla de compilación de ejecutable para prueba del driver
lmp_cafe: $(OBJ)
	$(CC) -o $@$(EEXT) $@.c $^ $(CFLAGS) -lm

lmp_robocol_test: $(OBJ)
	$(CC) -o $@$(EEXT) $@.c $^ $(CFLAGS)

include ../Makefile

