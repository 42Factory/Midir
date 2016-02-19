#include "Sensor_Box.h"

SensorBox sensorbox;

bool SensorBox::begin(String _SSID, String _WPA_KEY, String _TS_API_KEY)
{
	this->SSID = _SSID;
	this->WPA_KEY = _WPA_KEY;
	this->TS_API_KEY = _TS_API_KEY;
	
	Serial.begin(9600);
	
	SerialESP8266 = new SoftwareSerial(8, 9);
	SerialESP8266->begin(9600);
	
	delay(2000);
	
	if (!this->sendATCommand("AT", 10000, "OK"))
	{
		Serial.println("Unable to connect with the ESP8266");
		return false;
	}
	else
		Serial.println("Now connected to the ESP8266 !");
	
	this->sendATCommand("AT+RST", 10000, "ready");
	
	if(!this->connectWifi())
	{
		Serial.print("Unable to connect to the SSID ");
		Serial.println(this->SSID);
		return false;
	}
	else
	{
		Serial.print("Now connected to ");
		Serial.println(this->SSID);
	}
	
	return true;
}

void SensorBox::prepareRequest()
{
	request = "GET /update?api_key=" + this->TS_API_KEY;
}

void SensorBox::addData(int indexField, double value)
{
	String s_indexField = String(indexField);
	String s_value = String(value);
	
	request += "&field" + s_indexField + "=" + s_value;
}

bool SensorBox::sendRequest()
{
	String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80";

	if(!this->sendATCommand(cmd, 10000, "Linked || ALREAY CONNECT"))
	{
		Serial.print("Unable to reach the ThingSpeak server");
		return false;
	}
	
	String s_length = String(request.length() + 2);
	
	cmd = "AT+CIPSEND=" + s_length;
	
	if(!this->sendATCommand(cmd, 10000, ">"))
	{
		Serial.print("Unable to reach the ThingSpeak server");
		this->sendATCommand("AT+CIPCLOSE");
		return false;
	}
	
	if(!this->sendATCommand(request, 15000, "SEND OK"))
	{
		Serial.print("Unable to reach the ThingSpeak server");
		this->sendATCommand("AT+CIPCLOSE");
		return false;
	}
	
	Serial.print("Data sent to the ThingSpeak server !");
	
	return true;
}

bool SensorBox::connectWifi()
{
	this->sendATCommand("AT+CWMODE=1");

	String cmd = "AT+CWJAP=\"" + this->SSID + "\",\"" + this->WPA_KEY + "\"";

	if(!this->sendATCommand(cmd, 15000, "OK"))
		return false;
	
	return true;
}

bool SensorBox::sendATCommand(String ATcommand, int timeout, String expectedAnswer)
{
	while (SerialESP8266->available())
		SerialESP8266->read();    // Clean the input buffer

	Serial.println(ATcommand);
	
	SerialESP8266->println(ATcommand);

	if (expectedAnswer != "")
	{
		unsigned long previousTimer = millis();
		String response = "";

		// Tant que l'on a pas reçu la réponse voulu et que le timeout n'est pas écoulé
		while ((millis() - previousTimer) < timeout)
		{
			while (SerialESP8266->available())
			{
				response += (char)SerialESP8266->read();
				
				String tmpExpectedAnswer = expectedAnswer;
				String answer;
				int index;
				
				while (true)
				{
					index = tmpExpectedAnswer.indexOf(" || ");
					
					if (index > -1)
					{
						answer = tmpExpectedAnswer.substring(0, index);
						tmpExpectedAnswer = tmpExpectedAnswer.substring(index + 4);
					}
					else
						answer = tmpExpectedAnswer;

					if (response.indexOf(answer) > -1)
					{
						Serial.println(response);
						return true;
					}
					
					if (index == -1)
						break;
				}
			}
		}
		
		Serial.println(response);

		return false;
	}
	else
		return true;
}

void SensorBox::sendATCommand(String ATcommand)
{
	while (SerialESP8266->available())
		SerialESP8266->read();    // Clean the input buffer

	Serial.println(ATcommand);
	
	SerialESP8266->println(ATcommand);
}