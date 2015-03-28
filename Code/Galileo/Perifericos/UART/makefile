all: uart.g
uart.g: uart_robocol.h uart_robocol.c uart_test2.c
	galileo-gcc uart_test2.c uart_robocol.c -o uart.g
clean:
	rm uart.g