#include "Digital_Light_TSL2561.h"

void setup() 
{
	Serial.begin(9600);
	tsl2561.begin();
}
 
void loop() 
{
    long lux = tsl2561.readVisibleLux();
    
	if (lux_TSL2561 > -1)
    {
      Serial.print("LUX ");
      Serial.println(lux);
    }
	
	delay(1000);
}