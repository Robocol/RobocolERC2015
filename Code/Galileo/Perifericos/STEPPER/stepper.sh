#!/bin/bash
echo -n "31" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio31/direction
echo -n "strong" > /sys/class/gpio/gpio31/drive
echo -n "1" > /sys/class/gpio/gpio31/value
echo -n "31" > /sys/class/gpio/unexport

echo -n "32" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio32/direction
echo -n "strong" > /sys/class/gpio/gpio32/drive
echo -n "1" > /sys/class/gpio/gpio32/value
echo -n "32" > /sys/class/gpio/unexport

echo -n "30" > /sys/class/gpio/export
echo -n "out" > /sys/class/gpio/gpio30/direction
echo -n "strong" > /sys/class/gpio/gpio30/drive
echo -n "1" > /sys/class/gpio/gpio30/value
echo -n "30" > /sys/class/gpio/unexport

echo -n "3" > /sys/class/pwm/pwmchip0/export
echo -n "1" > /sys/class/pwm/pwmchip0/pwm3/enable
echo -n "2000000" > /sys/class/pwm/pwmchip0/pwm3/period
echo -n "3" > /sys/class/pwm/pwmchip0/unexport
echo -n "$1" > /sys/class/pwm/pwmchip0/pwm3/duty_cycle

