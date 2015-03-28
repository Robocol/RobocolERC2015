all: p_traccion.o
p_traccion.o: socket_send.c ph_robocol.c ph_robocol.h spi_robocol.c spi_robocol.h gpio_robocol.c gpio_robocol.h i2c_robocol.c i2c_robocol.h
	galileo-gcc socket_send.c ph_robocol.c spi_robocol.c gpio_robocol.c i2c_robocol.c -o p_traccion.o
clean:
	rm p_traccion.o