echo -n "29" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio29/direction
echo -n "strong" > /sys/class/gpio/gpio29/drive
echo -n "0" > /sys/class/gpio/gpio29/value
echo -n "29" > /sys/class/gpio/unexport
