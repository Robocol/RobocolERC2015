all: adc_test.g
adc_test.g: adc_test.c adc_robocol.c adc_robocol.h gpio_robocol.c gpio_robocol.h i2c_robocol.c i2c_robocol.h
	galileo-gcc adc_test.c adc_robocol.c gpio_robocol.c i2c_robocol.c -o adc_test.g
clean:
	rm adc_test.g