#Este script permite la conexion a router por medio de WiFi con protocolo WPA/WPA2
#Se asume que el archivo de configuracion /etc/wpa_supplicant.conf existe y 
#esta correctamente instanciado
#Establece la conexion al router en segundo plano
echo "iniciando conexion Wi-Fi"
wpa_supplicant -B -D wext -i wlan0 -c /etc/wpa_supplicant.conf

#Asigna IP estatica a la tarjeta. La IP debe estar reservada en el router para 
#evitar que sea asignada aotros dispositivos por el server DHCP
echo "Asignando direccion IP estatica: 192.168.0.101"
ifconfig wlan0 192.168.0.101 up

#Despues de correr este script, puede usarse el comando ping para corroborar
#que la conexion fue exitosaente establecida.
