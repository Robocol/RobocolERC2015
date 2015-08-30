################################################################################################
#	Definición de reglas de compilación de ejecutables con PH   							   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas ara el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../I2C \
       -I../GPIO\
       -I../../MISC

#Objetos de los cuales depende la compilación de los ejecutables STEPPER
_OBJ = i2c_robocol$(LEXT) \
       gpio_robocol$(LEXT)\
       misc_robocol$(LEXT)\
       adc_robocol$(LEXT)

#Concatenación de objetos con ruta de ubicación predeterminada
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

################################################################################################
#	Definición de reglas 																	   #
################################################################################################

#Regla de compilación de ejecutable para prueba del driver
prueba_adc: $(OBJ)
	$(CC) -o $@$(EEXT) adc_test.c $^ $(CFLAGS)

#Regla de compilación de ejecutable para prueba del driver
medicion_voltaje: $(OBJ)
	$(CC) -o $@$(EEXT) medicion_voltaje.c $^ $(CFLAGS)


include ../Makefile

