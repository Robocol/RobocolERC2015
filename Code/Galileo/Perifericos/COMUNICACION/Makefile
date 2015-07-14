################################################################################################
#	Definición de reglas de compilación de ejecutables con SERVIDOR   							   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas para el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../I2C \
       -I../GPIO\
       -I../SPI \
       -I../../MISC\
       -I../PH\
       -I../TRACCION\
       -I../COMUNICACION

#Objetos de los cuales depende la compilación de los ejecutables PH
_OBJ = spi_robocol$(LEXT) \
       i2c_robocol$(LEXT) \
       gpio_robocol$(LEXT)\
       ph_robocol$(LEXT) \
       traccion_robocol$(LEXT)\
       misc_robocol$(LEXT)\
       servidor_robocol$(LEXT)

_OBJ2= spi_robocol$(LEXT) \
       i2c_robocol$(LEXT) \
       gpio_robocol$(LEXT)\
       ph_robocol$(LEXT) \
       traccion_robocol$(LEXT)\
       misc_robocol$(LEXT)


#Concatenación de objetos con ruta de ubicación predeterminada
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
OBJ2= $(patsubst %,$(ODIR)/%,$(_OBJ2))

################################################################################################
#	Definición de reglas 																	   #
################################################################################################


all: test_servidor servidor_no_lib
#Regla de compilación de ejecutable para prueba del driver
test_servidor: $(OBJ)
	$(CC) -o $@$(EEXT) test_servidor.c $^ $(CFLAGS)

servidor_no_lib: $(OBJ2)
	$(CC) -o $@$(EEXT) servidor_no_lib.c $^ $(CFLAGS)

include ../Makefile