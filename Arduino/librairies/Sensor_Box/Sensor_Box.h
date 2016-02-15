#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SensorBox {

public:
  void begin(String hostname);
  void beginTransmission();
  void send(String name, double value);
  void endTransmission();
private:

};

 extern SensorBox sensorbox;
