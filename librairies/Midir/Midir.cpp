#include "Midir.h"

Midir midir;

bool Midir::begin(String _SSID, String _WPA_KEY, String _TS_SERVER, String _TS_API_KEY)
{
	SDConnected = SD.begin(SS);
	
	if (SDConnected)
	{
		Serial.println("SD Card detected !");
		
		DS3231 rtc(SDA, SCL);
		rtc.begin();
		
		TimeRTC t = rtc.getTimeRTC();
		
		setTime(t.hour, t.min, t.sec, t.date, t.mon, t.year);
		
		SD.mkdir("/MIDIR/");
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

void Midir::prepare()
{
	if (SDConnected)
		request = "[" + String(this->getDateStr()) + " " + String(this->getTimeStr()) + "]";
	else
		request = "GET /update?api_key=" + this->TS_API_KEY;
}

void Midir::addData(int indexField, String name, double value)
{
	String s_value = String(value);
	String s_indexField = String(indexField);
	
	if (SDConnected)
		request += " - " + name + ": " + s_value;
	else
		request += "&field" + s_indexField + "=" + s_value;
}

bool Midir::send()
{
	if (SDConnected)
	{
		String srcFile = "/MIDIR/" + String(this->getDateStr()) + ".txt";
		
		Serial.println(srcFile);
		
		File file = SD.open(srcFile, FILE_WRITE);

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
		
		String server_addr = this->TS_SERVER.substring(0, this->TS_SERVER.indexOf(':'));
		String server_port = this->TS_SERVER.substring(this->TS_SERVER.indexOf(':') + 1);

		String cmd = "AT+CIPSTART=\"TCP\",\"" + server_addr + "\"," + server_port;

		if(!this->sendATCommand(cmd, 10000, "Linked || ALREAY CONNECT"))
		{
			Serial.println("Unable to reach the ThingSpeak server");
			this->sendATCommand("AT+CIPCLOSE");
			return false;
		}
		
		//this->request += " HTTP/1.1 Host: " + this->TS_SERVER + " Connection: close Accept: */*";
		
		cmd = "AT+CIPSEND=" + String(request.length() + 2);
		
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

bool Midir::connectWifi()
{
	this->sendATCommand("AT+CWMODE=1");

	String cmd = "AT+CWJAP=\"" + this->SSID + "\",\"" + this->WPA_KEY + "\"";

	if(!this->sendATCommand(cmd, 15000, "OK"))
		return false;
	
	return true;
}

void Midir::sendATCommand(String ATcommand)
{
	this->sendATCommand(ATcommand, 0, "");
}

bool Midir::sendATCommand(String ATcommand, int timeout, String expectedAnswer)
{
	while (SerialESP8266->available())
		SerialESP8266->read();

	Serial.println(ATcommand);
	
	SerialESP8266->println(ATcommand);

	if (expectedAnswer != "")
	{
		unsigned long previousTimer = millis();
		String response = "";

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

void Midir::resetESP()
{
	digitalWrite(PIN_ESP_RESET, LOW);
	delay(100);
	digitalWrite(PIN_ESP_RESET, HIGH);
}

char* Midir::getTimeStr()
{	
	static char output[] = "xxxxxxxx";

	if (hour()<10)
		output[0]=48;
	else
		output[0]=char((hour() / 10)+48);
	output[1]=char((hour() % 10)+48);
	output[2]=58;
	if (minute()<10)
		output[3]=48;
	else
		output[3]=char((minute() / 10)+48);
	output[4]=char((minute() % 10)+48);
	output[5]=58;
	
	if (second()<10)
		output[6]=48;
	else
		output[6]=char((second() / 10)+48);
	output[7]=char((second() % 10)+48);
	output[8]=0;
	
	return (char*)&output;
}

char* Midir::getDateStr()
{
	static char output[] = "xxxxxxxxxx";
	char divider = '-';
	int yr, offset;

	if (day()<10)
		output[0]=48;
	else
		output[0]=char((day() / 10)+48);
	output[1]=char((day() % 10)+48);
	output[2]=divider;
	if (month()<10)
		output[3]=48;
	else
		output[3]=char((month() / 10)+48);
	output[4]=char((month() % 10)+48);
	output[5]=divider;
	
	yr=year()-2000;
	if (yr<10)
		output[6]=48;
	else
		output[6]=char((yr / 10)+48);
	output[7]=char((yr % 10)+48);
	output[8]=0;
		
	return (char*)&output;
}
