#include "HTU21DF.h"

void setup() 
{
	Serial.begin(9600);
	htu21df.begin();
}
 
void loop() 
{
    double humidity = htu21df.readHumidity();
    double temperature = htu21df.readTemperature();
    
	// 999 : CRC ERROR
	// 998 : Timeout ERROR
	if (humidity_HTU21DF < 998)
    {
      Serial.print("HUMIDITY ");
      Serial.println(humidity);
    }

    if (temperature_HTU21DF < 998)
    {
      Serial.print("TEMPERATURE ");
      Serial.println(temperature);
    }
	
	delay(1000);
}

