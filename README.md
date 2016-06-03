# ESP-8266-Azure

Příklady - napojení ESP8266 (Wemos D1 mini) s čidlem na měření teploty (DHT22 nebo DHT11) na Azure IoT Hub

- ESPIOTHUBHTTPS - posílání přes HTTPS
- ESPIOTHUBMQTT - posílání pomocí protokolu MQTT + callback pro posílání zpráv do zařízení

## Instalace

### Instalace Arduino IDE (1.6.9)
https://www.arduino.cc/en/Main/Software

File - Preferences - Aditional Board Manager Url
http://arduino.esp8266.com/stable/package_esp8266com_index.json

Sketch - Include Library - Manage Libraries
Vyhledat DHT
DHT Sensor Library by Adafruit - Install

### Device explorer
https://github.com/Azure/azure-iot-sdks/blob/master/tools/DeviceExplorer/doc/how_to_use_device_explorer.md

- zkopírovat IoT Hub Owner conenction string z Azure Portálu
- vytvořit zařízení
- u zařízení získat SAS token
- nakopírovat do kódu

### SAS generator (pro lidi bez Windows)
https://iot.pospa.net/IotHub/Device - nejdou zadavat TTL (chyba stránky), ale da se rucne, je to v minutach

lepsi si udelat VM v azure a tam si nainstalovat Device Explorer


Kód:
	- Wifi je nastavena na router a je spravne
	- čidla
		○ DHT11 - modrý
		○ DHT22 - bílý
