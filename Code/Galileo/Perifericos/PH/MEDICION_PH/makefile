all:logvc_ph.g
logvc_ph.g:logvc_ph.c ph_robocol.h ph_robocol.c spi_robocol.h spi_robocol.c gpio_robocol.h gpio_robocol.c i2c_robocol.h i2c_robocol.c
	galileo-gcc logvc_ph.c ph_robocol.c spi_robocol.c gpio_robocol.c i2c_robocol.c -o logvc_ph.g
clean:
	rm logvc_ph.g