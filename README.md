Midir Project
==========

Arduino based simple and modulable system to get environments information using multiple sensors

### Consists of :
1. Arduino gets informations from sensors
2. If a SD card is plugged, the arduino write them on it
3. Else, it send them to a ThingSpeak server trough an ESP8266 Wi-Fi module

### Components
- In our case, an **Arduino Mega 2560** was used
- To supply enough current to feed the ESP8266 (200mA needed vs 30mA providen by the 3v3 Mega's pin) and to make sure to get a regular 3v3 and 5v circuits for our sensor, we used a **MB102** regulator and a 9V/2A power supply to serve our system.
- An **ESP8266** module to send datas to a ThingSpeak server trough Wi-Fi
- A **SD Card** module to save datas if no Wi-Fi network is available
- And **RTC DS3231** module to keep the clock time and write it on the SD Card with datas

![Midir Box](http://img4.hostingpics.net/pics/360237allinone.jpg)
![Midir Box](http://img4.hostingpics.net/pics/51351820160304115252.jpg)
_First version of the Midir_

### Wiring schema

![Midir Wiring Schema](http://img11.hostingpics.net/pics/584818Sensorboxbb.png)

Note : We try first to wire the SD card module to the 5v as it have a 5v pin, but it seems having trouble with that. We try to wire it to the 3v3 deliver by the MB102 regulator, but it cause irregular voltage and so trouble to read it. The only solution was to wire it to the 3v3 pin on the Arduino Mega and so it working as well.

### Repositery
|Directory                 |Description                                                |
|--------------------------|-----------------------------------------------------------|
|libraries                 |Sensors libraires and datasheet                            |
|libraries/Midir           |Midir librairy used to send datas                          |
|midir.ino                 |Arduino sketch main program                                |

### Basic Sensors
|Sensor         |Type                                  |
|---------------|--------------------------------------|
|HTU21D         |Temperature (C°)/Humidity (%)         |
|HYSRF05        |Presence (cm)                         |
|LM386          |Noise (dB)                            |
|TSL2561T       |Light (lux)                           |

#### Partners on this project :
[42 Factory](http://42factory.com)  
[Stereolux Nantes](http://www.stereolux.org)
