#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h>

class SensorBox {

private:
	SoftwareSerial *SerialESP8266;
	String SSID;
	String WPA_KEY;
	String TS_API_KEY;
	String request;
	bool sendATCommand(String ATcommand, int timeout, String expectedAnswer);
	void sendATCommand(String ATcommand);
	bool connectWifi();
public:
	bool begin(String _SSID, String _WPA_KEY, String _TS_API_KEY);
	void prepareRequest();
	void addData(int indexField, double value);
	bool sendRequest();
};

 extern SensorBox sensorbox;
