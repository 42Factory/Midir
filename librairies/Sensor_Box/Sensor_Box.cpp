#include "Sensor_Box.h"

SensorBox sensorbox;

bool SensorBox::begin(String _SSID, String _WPA_KEY, String _TS_SERVER, String _TS_API_KEY)
{
	SDConnected = SD.begin(SS);
	
	if (SDConnected)
	{
		rtc = new DS3231(SDA, SCL);
		rtc->begin();
		
		Serial.println("SD Card detected !");
		
		SD.mkdir("/SENSB/");
	}
	else
	{
		this->SSID = _SSID;
		this->WPA_KEY = _WPA_KEY;
		this->TS_SERVER = _TS_SERVER;
		this->TS_API_KEY = _TS_API_KEY;
		
		digitalWrite(PIN_ESP_RESET, HIGH);
		
		//SerialESP8266 = new SoftwareSerial(8, 9);
		SerialESP8266 = &Serial3;
		SerialESP8266->begin(9600);
		
		delay(2000);
		
		if (!this->sendATCommand("AT", 10000, "OK"))
		{
			this->resetESP();
			
			delay(2000);
			
			if (!this->sendATCommand("AT", 10000, "OK"))
			{
				Serial.println("Unable to connect with the ESP8266");
				return false;
			}
		}
		
		Serial.println("Now connected to the ESP8266 !");
		
		this->sendATCommand("AT+RST", 10000, "ready");
		
		wifiConnected = this->connectWifi();
		
		if(!wifiConnected)
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
	}
	
	return true;
}

void SensorBox::prepare()
{
	if (SDConnected)
	{
		String s_date = String(rtc->getDateStr());
		String s_time = String(rtc->getTimeStr());
		
		request = "[" + s_date + " " + s_time + "]";
	}
	else
		request = "GET /update?api_key=" + this->TS_API_KEY;
}

void SensorBox::addData(int indexField, String name, double value)
{
	String s_value = String(value);
	String s_indexField = String(indexField);
	
	if (SDConnected)
		request += " - " + name + ": " + s_value;
	else
		request += "&field" + s_indexField + "=" + s_value;
}

bool SensorBox::send()
{
	Serial.println("==1==");
	Serial.println(rtc->getTimeStr());
	Serial.println("==2==");
	
	if (SDConnected)
	{
		String rtcDate = String(rtc->getDateStr(FORMAT_SHORT));
		rtcDate.replace('.', '-');
		
		String srcFile = "/SENSB/" + rtcDate + ".txt"; // "SENSORBOX/" + 
		 
		Serial.println(srcFile);
		
		File file = SD.open(srcFile, FILE_WRITE); // srcFile.c_str()

		if (file)
		{
			file.println(request);
			file.close();
			
			Serial.println("Data wrote on the SD Card !");
		}
		else
			Serial.println("Error while writing on the SD Card");
	}
	else
	{
		if(!wifiConnected)
			return false;
		
		String cmd = "AT+CIPSTART=\"TCP\",\"" + this->TS_SERVER + "\",80";

		if(!this->sendATCommand(cmd, 10000, "Linked || ALREAY CONNECT"))
		{
			Serial.println("Unable to reach the ThingSpeak server");
			return false;
		}
		
		String s_length = String(request.length() + 2);
		
		cmd = "AT+CIPSEND=" + s_length;
		
		if(!this->sendATCommand(cmd, 10000, ">"))
		{
			Serial.println("Unable to reach the ThingSpeak server");
			this->sendATCommand("AT+CIPCLOSE");
			return false;
		}
		
		if(!this->sendATCommand(request, 15000, "SEND OK"))
		{
			Serial.println("Unable to reach the ThingSpeak server");
			this->sendATCommand("AT+CIPCLOSE");
			return false;
		}
		
		Serial.println("Data sent to the ThingSpeak server !");
	}
	
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

void SensorBox::sendATCommand(String ATcommand)
{
	this->sendATCommand(ATcommand, 0, "");
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

void SensorBox::resetESP()
{
	digitalWrite(PIN_ESP_RESET, LOW);
	delay(100);
	digitalWrite(PIN_ESP_RESET, HIGH);
}