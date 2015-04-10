################################################################################################
#	FORMATO DE REGLAS																		   #
################################################################################################
#	Variables previamente definidas:
#		ODIR: Directorio de almacenamiento de los object files .go (Galileo Object)
#
#$(ODIR)/foo.go: $(ROUTETOFOO)/foo.c $(ROUTETOFOO)/foo.h $(ODIR)/object_needed1.go \
#            										    $(ODIR)/object_needed2.go
#	gcc -c -o $@ $< -I $(ROUTETOFOO)
################################################################################################

################################################################################################
#	I2C  																			   		   #
################################################################################################

$(ODIR)/i2c_robocol$(LEXT): ../I2C/i2c_robocol.c ../I2C/i2c_robocol.h
	$(CC) -c -o $@ $< -I../I2C



################################################################################################
#	GPIO 																			   		   #
################################################################################################
$(ODIR)/gpio_robocol$(LEXT): ../GPIO/gpio_robocol.c ../GPIO/gpio_robocol.h $(ODIR)/i2c_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
                    -I../I2C



################################################################################################
#	SPI 																			   		   #
################################################################################################
$(ODIR)/spi_robocol$(LEXT): ../SPI/spi_robocol.c ../SPI/spi_robocol.h $(ODIR)/i2c_robocol$(LEXT) \
                                                                 $(ODIR)/gpio_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
                    -I../I2C  \
                    -I../SPI



################################################################################################
#	PUENTE H 																			   	   #
################################################################################################
$(ODIR)/ph_robocol$(LEXT): ../PH/ph_robocol.c ../PH/ph_robocol.h $(ODIR)/i2c_robocol$(LEXT)   \
                                                            $(ODIR)/gpio_robocol$(LEXT)   \
                                                            $(ODIR)/spi_robocol$(LEXT)
	$(CC) -c -o $@ $< -I../GPIO \
                      -I../I2C  \
                      -I../SPI  \
                      -I../PH



.PHONY: clean

clean:
	rm -f $(ODIR)/*$(LEXT) ./*$(EEXT) ./*~