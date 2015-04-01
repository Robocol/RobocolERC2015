#Exportando los puertos que serán modificados a sysFS. Estos puertos son las señales de control 
#de los multiplexores que seleccionan entre los diferentes usos de un pin

echo -n "4" > /sys/class/gpio/export
echo -n "42" > /sys/class/gpio/export
echo -n "43" > /sys/class/gpio/export
echo -n "54" > /sys/class/gpio/export
echo -n "55" > /sys/class/gpio/export


#Se asigna dirección de salida a los puertos para que el valor de estos pueda ser modificado internamente
echo -n "out" > /sys/class/gpio/gpio4/direction
echo -n "out" > /sys/class/gpio/gpio42/direction
echo -n "out" > /sys/class/gpio/gpio43/direction
echo -n "out" > /sys/class/gpio/gpio54/direction
echo -n "out" > /sys/class/gpio/gpio55/direction


#Drive modes para las salidas
echo -n "strong" > /sys/class/gpio/gpio42/drive
echo -n "strong" > /sys/class/gpio/gpio43/drive
echo -n "strong" > /sys/class/gpio/gpio54/drive
echo -n "strong" > /sys/class/gpio/gpio55/drive

#Asignación de valor a los selectores de los multiplexores.
echo -n "1" > /sys/class/gpio/gpio4/value
echo -n "0" > /sys/class/gpio/gpio42/value
echo -n "0" > /sys/class/gpio/gpio43/value
echo -n "0" > /sys/class/gpio/gpio54/value
echo -n "0" > /sys/class/gpio/gpio55/value

#Retirando los puertos de SysFS
echo -n "4" > /sys/class/gpio/unexport
echo -n "42" > /sys/class/gpio/unexport
echo -n "43" > /sys/class/gpio/unexport
echo -n "54" > /sys/class/gpio/unexport
echo -n "55" > /sys/class/gpio/unexport
