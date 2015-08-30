#sudo apt-get install --install-recommends jstest* joystick xboxdrv #Comentar luego de la primera ejecucion
sudo echo "blacklist xpad" | sudo tee -a /etc/modprobe.d/blacklist.conf
sudo rmmod xpad
cd ~/xboxdrv
sudo xboxdrv --silent --config ROBOCOL.xboxdrv
