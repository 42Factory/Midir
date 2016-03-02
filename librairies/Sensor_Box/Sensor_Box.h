#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h>
#include <DS3231.h>
#include <SD.h>
#include <Time.h>

#define PIN_ESP_RESET 22

class SensorBox {

private:
  //SoftwareSerial *SerialESP8266;
  HardwareSerial *SerialESP8266;
  String SSID;
  String WPA_KEY;
  String TS_SERVER;
  String TS_API_KEY;
  String request;
  bool wifiConnected;
  bool SDConnected;
  bool sendATCommand(String ATcommand, int timeout, String expectedAnswer);
  void sendATCommand(String ATcommand);
  bool connectWifi();
  void resetESP();
  char* getTimeStr();
  char* getDateStr();
public:
  bool begin(String _SSID, String _WPA_KEY, String _TS_SERVER, String _TS_API_KEY);
  bool isConnected() { return wifiConnected; }
  void prepare();
  void addData(int indexField, String name, double value);
  bool send();
};

 extern SensorBox sensorbox;

