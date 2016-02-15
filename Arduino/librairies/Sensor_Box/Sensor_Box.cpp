#include "Sensor_Box.h"

SensorBox sensorbox;



void SensorBox::begin(String hostname)
{
  /* Opening USB Serial connection with the raspberry */
  Serial.begin(9600);

  /* Sending initialise settings to the raspberry */
  Serial.println("SETUP");
  Serial.print("HOSTNAME ");
  Serial.println(hostname);
}

void SensorBox::beginTransmission()
{
  Serial.println("BEGIN");
}

void SensorBox::send(String name, double value)
{
  String data = name + " " + value;
  Serial.println(data);
}

void SensorBox::endTransmission()
{
  Serial.println("END");
}
