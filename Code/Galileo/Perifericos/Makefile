################################################################################################
#	MAKEFILE PERIFERICOS														 			   #
################################################################################################
#
# Este archivo debe ser incluido dentro de cada uno de los makefiles escritos para los diferentes
# programas presentes en la carpeta perifericos. Dicha acción se realiza por medio de la
# instruccion: include ../Makefile al final de los diferentes Makefiles.
#
# Previa a la inclusion de este archivo dentro de cualquier Makefile se debe realizar la adicion
# del archivo de configuracion .config en donde se encuentran definidas variables indispensables
# para el correcto funcionamiento de los diferentes Makefiles que utilicen la estructura definida
# en el presente documento.
#
#
# %%%% VARIABLES PREDEFINIDAS EN .config %%%%
#
#		ODIR: Directorio de almacenamiento de los object files .og (Galileo Object) que se
#			  compilen tras la ejecucion exitosa de este programa. Los Galileo Objects son
#			  tan solo un nombre definido por los desarrolladores Robocol para los precompilados
#			  de las diferentes librerias.
#
#		CC: Compilador de c a utilizar. Para el proyecto Robocol URC, dado que se esta utilizando
#			tarjetas de desarrollo Intel Galileo el compilador estipulado por defecto es:
#
#		     		i586-poky-linux-uclibc-gcc
#
#			Si desea realizar el cambio del compilador a utilizar dirijase al archivo .config y
#			efectue la modificacion.
#			
#		EEXT: Extension de archivo ejecutable definido por los desarrolladores Robocol. Por defecto
#			  dicha extension es:
#					
#					.eg
#
#		LEXT: Extension de archivo precompilado de libreria o Galileo object. Por defecto se
#			  utiliza la extension:
#					
#					.og
#		
#		ROUTEFOO: Es la ruta definida para la busqueda de los elementos a compilar.Corresponde
#				  a cada una de los archivos de C que describen las funcionalidades de las
#				  librerias.
#
# %%%% FORMATO DE LAS REGLAS DEL MAKEFILE %%%%
#
#	A continuacion se presenta la estructura de la regla de compilacion definida en
#	este Makefile. Se espera se mantenga este formato dentro del resto de Makefiles a definir
#	para el proyecto ERC Robocol 2015.
#	
# ---------------------------------------------------------------------------------------------------------
#	$(ODIR)/foo.$(EEXT o LEXT): $(ROUTETOFOO)/foo.c $(ROUTETOFOO)/foo.h $(ODIR)/object_needed1.$(LEXT) \
#            										   	 $(ODIR)/object_needed2.$(EEXT) ...
#		$(CC) -c -o $@ $< -I $(ROUTETOFOO)
# --------------------------------------------------------------------------------------------------------
#	
#	A seguir se describen los elementos interpuestos en la regla de compilacion:
#	
#	$(ODIR)/foo.og: Se refiere al objeto final de la regla de compilacion. En este caso el nombre
#					del objeto es foo.$(EEXT) y se crea o se encuentra en la carpeta definida por ODIR.
#
#	$(ROUTETOFOO)/foo.c: Se refiere al archivo en C a compilar.
#
#	$(ROUTETOFOO)/foo.h: Es el header correspondiente al archivo en C a compilar.
#
#	$(ODIR)/object_needed1.$(LEXT): Objetos precompilados de libreria necesarios para completar exitosamente
#									la compilacion del elemento onjetivo.
#			
#	$@ : Utilidad del Makefile que representa el objetivo de la regla, i.e. , el archivo a compilar.7
#
#	$< : Utilidad del Makefile que representa el primer prerrequisito de la regla en estudio.
#
#	-I $(ROUTETOFOO): Flag de compilacion que indica al compilador de C donde realizar la busqueda de dependencias
#					 que no se cumplan con los archivos presentes en la carpeta donde se realiza la ejecucion del
#					 Makefile.
#
################################################################################################

################################################################################################
#	I2C  																			   		   #
################################################################################################

$(ODIR)/i2c_robocol$(LEXT): ../I2C/i2c_robocol.c ../I2C/i2c_robocol.h
	$(CC) -c -o $@ $< -I../I2C



################################################################################################
#	GPIO 																			   		   #
################################################################################################
$(ODIR)/gpio_robocol$(LEXT): ../GPIO/gpio_robocol.c ../GPIO/gpio_robocol.h $(ODIR)/i2c_robocol$(LEXT)\
																		   $(ODIR)/misc_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
                   	  -I../I2C \
                   	  -I../../MISC

################################################################################################
#	GPIO 																			   		   #
################################################################################################
$(ODIR)/uart_robocol$(LEXT): ../UART/uart_robocol.c ../UART/uart_robocol.h $(ODIR)/misc_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../UART \
                   	  -I../../MISC


################################################################################################
#	SPI 																			   		   #
################################################################################################
$(ODIR)/spi_robocol$(LEXT): ../SPI/spi_robocol.c ../SPI/spi_robocol.h $(ODIR)/i2c_robocol$(LEXT) \
                                                                 $(ODIR)/gpio_robocol$(LEXT) \
	    		                                           		 $(ODIR)/misc_robocol$(LEXT)

	$(CC) -c -o $@ $< -I../GPIO \
                   	  -I../I2C 	\
                      -I../SPI  \
                      -I../../MISC


################################################################################################
#	PUENTE H 																			   	   #
################################################################################################
$(ODIR)/ph_robocol$(LEXT): ../PH/ph_robocol.c ../PH/ph_robocol.h $(ODIR)/i2c_robocol$(LEXT)   \
                                                            $(ODIR)/gpio_robocol$(LEXT)   \
                                                            $(ODIR)/spi_robocol$(LEXT)	  \
 		                                           			$(ODIR)/misc_robocol$(LEXT)

	$(CC) -c -o $@ $< -I../GPIO \
                      -I../I2C  \
                      -I../SPI  \
                      -I../PH 	\
                      -I../../MISC

################################################################################################
#	ADC																					   	   #
################################################################################################

$(ODIR)/adc_robocol$(LEXT): ../ADC/adc_robocol.c ../ADC/adc_robocol.h $(ODIR)/gpio_robocol$(LEXT)\
																	$(ODIR)/misc_robocol$(LEXT)\
																	$(ODIR)/i2c_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../../MISC\
					-I../GPIO\
					-I../I2C 


################################################################################################
#	MISC 																			   	   	   #
################################################################################################

$(ODIR)/misc_robocol$(LEXT): ../../MISC/misc_robocol.c ../../MISC/misc_robocol.h
	$(CC) -c -o $@ $<

################################################################################################
#	PWM 																			   	   	   #
################################################################################################

$(ODIR)/pwmg_robocol$(LEXT): ../PWM/pwmg_robocol.c ../PWM/pwmg_robocol.h $(ODIR)/i2c_robocol$(LEXT)   \
                                                            			 $(ODIR)/gpio_robocol$(LEXT)  \
                                                            			 $(ODIR)/misc_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
                      -I../I2C  \
                      -I../../MISC


################################################################################################
#	STEPPER 																			   	   #
################################################################################################

$(ODIR)/stepper_robocol$(LEXT): ../STEPPER/stepper_robocol.c ../STEPPER/stepper_robocol.h $(ODIR)/i2c_robocol$(LEXT) \
       																						 $(ODIR)/gpio_robocol$(LEXT)\
       																						 $(ODIR)/spi_robocol$(LEXT) \
       																						 $(ODIR)/misc_robocol$(LEXT)\
       																						 $(ODIR)/pwmg_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
                      -I../I2C  \
                      -I../SPI  \
                      -I../../MISC\
                      -I../PWM -lm

################################################################################################
#	LMP																					   	   #
################################################################################################

$(ODIR)/lmp_robocol$(LEXT): ../TEMPERATURA/lmp_robocol.c ../TEMPERATURA/lmp_robocol.h $(ODIR)/i2c_robocol$(LEXT) \
       																						 $(ODIR)/gpio_robocol$(LEXT)\
       																						 $(ODIR)/spi_robocol$(LEXT) \
       																						 $(ODIR)/misc_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
                      -I../I2C  \
                      -I../SPI  \
                      -I../../MISC \
                      -I../TEMPERATURA

################################################################################################
#	ARM																					   	   #
################################################################################################

$(ODIR)/arm_robocol$(LEXT): ../BRAZO/arm_robocol.c ../BRAZO/arm_robocol.h  $(ODIR)/i2c_robocol$(LEXT)  \
                                                            					$(ODIR)/gpio_robocol$(LEXT) \
                                                            					$(ODIR)/spi_robocol$(LEXT)	\
 		                                           								$(ODIR)/misc_robocol$(LEXT) \
       																			$(ODIR)/pwmg_robocol$(LEXT) \
       																			$(ODIR)/ph_robocol$(LEXT) \
       																			$(ODIR)/stepper_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
						-I../I2C  \
						-I../SPI  \
						-I../../MISC \
						-I../PH \
						-I../PWM \
						-I../STEPPER \
						-I../BRAZO
################################################################################################
#	STEPPER																				   	   #
################################################################################################

$(ODIR)/traccion_robocol$(LEXT): ../TRACCION/traccion_robocol.c ../TRACCION/traccion_robocol.h  $(ODIR)/i2c_robocol$(LEXT)  \
                                                            					$(ODIR)/gpio_robocol$(LEXT) \
                                                            					$(ODIR)/spi_robocol$(LEXT)	\
 		                                           								$(ODIR)/misc_robocol$(LEXT) \
       																			$(ODIR)/pwmg_robocol$(LEXT) \
       																			$(ODIR)/ph_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
						-I../I2C  \
						-I../SPI  \
						-I../../MISC \
						-I../PH \
						-I../PWM \
                        -I../TRACCION

################################################################################################
#	SERVIDOR     																		   	   #
################################################################################################


$(ODIR)/servidor_robocol$(LEXT): ../COMUNICACION/servidor_robocol.c ../COMUNICACION/servidor_robocol.h  $(ODIR)/i2c_robocol$(LEXT)  \
                                                            					$(ODIR)/gpio_robocol$(LEXT) \
                                                            					$(ODIR)/spi_robocol$(LEXT)	\
 		                                           								$(ODIR)/misc_robocol$(LEXT) \
       																			$(ODIR)/pwmg_robocol$(LEXT) \
       																			$(ODIR)/ph_robocol$(LEXT)	\
       																			$(ODIR)/traccion_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
						-I../I2C  \
						-I../SPI  \
						-I../../MISC \
						-I../PH \
						-I../PWM \
                        -I../TRACCION\
                        -I../COMUNICACION   																			

################################################################################################
#	GPS    																		   	   #
################################################################################################


$(ODIR)/gps_robocol$(LEXT): ../GPS/gps_robocol.c ../GPS/gps_robocol.h  $(ODIR)/uart_robocol$(LEXT)  \
 		                                           								$(ODIR)/misc_robocol$(LEXT) 
	$(CC) -c -o $@ $< -I../UART \
						-I../../MISC   

.PHONY: clean

clean:
	rm -f $(ODIR)/*$(LEXT) ./*$(EEXT) ./*~