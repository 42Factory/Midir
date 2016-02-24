Sensor-Box 
==========

Arduino based simple and modulable system to get environment information using multiple sensors

### Consists of :
1. Arduino gets information from sensors
2. Send them to ThingSpeak server though esp8266 Wi-Fi module

### Basic Sensors
|Sensor         |Type                                  |
|---------------|--------------------------------------|
|HTU21D         |Temperature/Humidity                  |
|HYSRF05        |Distance by ultrasonic                |
|LM386          |Loudness                              |
|TSL2561T       |Light (lux)                           |

In our case, an Arduino Nano v3 was used

|Directory                 |Description                                                |
|--------------------------|-----------------------------------------------------------|
|librairies                 |Sensors libraires and datasheet                            |
|librairies/Sensor_Box/    |SensorBox librairy used to communicate with the raspberry  |
|sensorbox.ino             |Arduino sketch main program                                |

F.A.Q
====================

## How do I edit the program on the Arduino ?

To edit an Arduino sketch, you will need the Arduino IDE available at this address : [https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software "Arduino IDE Download")
Download and install it.

If it's your first time manipulating Arduino systems, we recommand you to follow these **Getting started** steps on the [Arduino Official Website](https://www.arduino.cc/en/Main/Software "Arduino Getting started")

Open the **sensorbox.ino** file and start editing.

## How do I add my sensor to the Sensorbox on the Arduino ?

First, you need to know if your sensor is an analogic, I2C, UART, (etc.) sensor.
Referr to the constructor page and try to get information about how does it work.

For example, we will add a **BMP180** Barometric Pressure/Temperature/Altitude Sensor bought on [Adafruit](https://www.arduino.cc/en/Main/Software "Adafruit BMP180") to our SensorBox

### Connection

According to the BMP180 Adafruit page, this board is 5V compliant - a 3.3V regulator and a i2c level shifter circuit is included so you can use this sensor safely with 5V logic and power.
Connect the VIN pin to the 5V voltage pin, GND to ground, SCL to I2C Clock (Analog 5) and SDA to I2C Data (Analog 4)

### Library

Download the Adafruit BMP085/BMP180 Arduino library and example code for temperature, pressure and altitude calculation [**Here**](https://github.com/adafruit/Adafruit-BMP085-Library "Adafruit BMP085/BMP180 Library") (Click on **Download ZIP**)
Unzip it and move it into your installation folder.
First, we will test our sensor apart, so load the example sketch (**.ino** file).
Then install the library :

![Importing Arduino Library](http://www.codevista.net/wp-content/uploads/2015/08/Importing-Arduino-Library.png "Importing Arduino Library")

You should have something like this at this point : 

You should have precision temperature, pressure and altitude data in the Serial Monitor

### Thingspeak

Installation on clean install of Ubuntu 14.04 LTS

```
#!shell

sudo apt-get upgrade

```

```
#!shell

sudo apt-get -y install build-essential mysql-server mysql-client libmysqlclient-dev libxml2-dev libxslt-dev git-core curl ruby

```

```
#!shell

gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3

```

```
#!shell

\curl -sSL https://get.rvm.io | bash -s stable

```

**Restart shell**


```
#!shell

rvm install 2.1
```


```
#!shell

git clone https://github.com/iobridge/thingspeak.git

```

```
#!shell

cd thingspeak

```

```
#!shell

sudo apt-get install bundler

```

**Edit Gemfile**
change line *gem 'ZenTest'* by *gem 'ZenTest', '4.8.3'*

```
#!shell


bundle install
```


```
#!shell

cp config/database.yml.example config/database.yml

```

**Edit config/database.yml**
change user passwork with your credit


```
#!shell

rake db:create
```


```
#!shell

rake db:schema:load

```

```
#!shell

rails server
```