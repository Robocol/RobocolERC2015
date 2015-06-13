################################################################################################
#	Definición de reglas de compilación de ejecutables con STEPPER							   #
################################################################################################

#Inclusión de Archivo general de configuración de compilación
include ../.config

#Banderas ara el compilador. Generalmente inclusión de los directorios donde buscar los .h
CFLAGS=-I../I2C\
		-I../GPIO\
		-I../PWM\
		-I../SPI\
		-I../../MISC\
		-I../PH\
		-I../STEPPER 

#Objetos de los cuales depende la compilación de los ejecutables SPI
_OBJ = stepper_robocol$(LEXT)\
	   spi_robocol$(LEXT)\
       i2c_robocol$(LEXT)\
       gpio_robocol$(LEXT)\
       misc_robocol$(LEXT)\
       pwmg_robocol$(LEXT)\
       arm_robocol$(LEXT)\
       ph_robocol$(LEXT)

#Concatenación de objetos con ruta de ubicación predeterminada
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

################################################################################################
#	Definición de reglas 																	   #
################################################################################################

#Regla de compilación de ejecutable para prueba del driver
prueba_arm: $(OBJ)
	$(CC) -o $@$(EEXT) prueba_arm.c $^ $(CFLAGS) -lm
prueba_arm_pablo: $(OBJ)
	$(CC) -o $@$(EEXT) prueba_arm_pablo.c $^ $(CFLAGS) -lm

include ../Makefile

