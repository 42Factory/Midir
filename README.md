Midir Project
==========

Arduino based simple and modulable system to get environments information using multiple sensors

### Consists of :
1. Arduino gets informations from sensors
2. If a SD card is plugged, the arduino write them on it
3. Else, it send them to a ThingSpeak server trough an ESP8266 Wi-Fi module

### Components
- In our case, an Arduino Mega 2560 was used
- To supply enough current to feed the ESP8266 (200mA needed vs 30mA providen by the 3v3 Mega's pin) and to make sure to get a regular 3v3 and 5v circuits for our sensor, we used a MB102 regulator and a 9V/2A power supply to serve our system.
- An ESP8266 module to send datas to a ThingSpeak server trough Wi-Fi
- A SD Card module to save datas if no Wi-Fi network is available
- And RTC DS3231 module to keep the clock time and write it on the SD Card with datas


### Basic Sensors
|Sensor         |Type                                  |
|---------------|--------------------------------------|
|HTU21D         |Temperature (C°)/Humidity (%)         |
|HYSRF05        |Presence (cm)                         |
|LM386          |Noise (dB)                            |
|TSL2561T       |Light (lux)                           |


|Directory                 |Description                                                |
|--------------------------|-----------------------------------------------------------|
|librairies                |Sensors libraires and datasheet                            |
|librairies/Midir          |Midir librairy used to send datas                          |
|midir.ino                 |Arduino sketch main program                                |



#### Partners on this project :
[42 Factory](http://42factory.com)
[Stereolux Nantes](http://www.stereolux.org)
