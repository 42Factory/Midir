Sensor-Box 
==========

Arduino-Raspberry based simple and modulable system to get environment information using multiple sensors

### Consists of :
1. Arduino gets information from sensor
2. It sending them to raspberry with USB Serial connection
3. Raspberry send received datas to a Firebase

### Basic Sensors
|Sensor         |Type                                  |
|---------------|--------------------------------------|
|HTU21D         |Temperature/Humidity                  |
|HYSRF05        |Distance by ultrasonic                |
|LM386          |Loudness                              |
|TSL2561T       |Light (lux)                           |

### Directories
|Directory      |Description                                         |
|---------------|----------------------------------------------------|
|Arduino        |C++ arduino file and librairies                     |
|Raspberry      |Script and installation and setup instructions      |
