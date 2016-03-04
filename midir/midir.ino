#include "Midir.h"

// Importing needed librairies for I2C digital sensor */
#include "Digital_Light_TSL2561.h"
#include "HTU21DF.h"

// Wifi settings
#define WIFI_SSID "42-RPI-Midir"
#define WIFI_WPA_KEY "42rpimidir"

// ThingSpeak settings
#define THINGSPEAK_SERVER "192.168.42.254:3000"
#define THINGSPEAK_API_KEY "Q1ADLJ5D5E3RKHRE"

// Fields for Thingspeaks Channel from 1 to 8
#define FIELD_TEMPERATURE 1
#define FIELD_HUMIDITY 2
#define FIELD_PRESENCE 3
#define FIELD_SOUND 4
#define FIELD_LIGHT 5
//#define FIELD_FOR_MY_SENSOR YOUR_INDEX_FIELD

// Program settings
#define DEBUG true
#define TIME_SLEEP 10           // Time in ms between each loop
#define PUBLISH 100             // Number of loop between each transmission of datas
#define ADC_VALUE 4600          // Votlage measured on 5V pin

// Here we define PINS on which analog sensor are connected to
#define PIN_HYSRF05_PRESENCE_ECHO 8
#define PIN_HYSRF05_PRESENCE_TRIGGER 9
#define PIN_LM358_SOUND A2

// Required to define optionnal parameter
// To measure distance from the HYSRF05, the 
long read_HYSRF05_PRESENCE(double temperature = 23.00);

// Declaring variables
long values_HYSRF05_PRESENCE = 0;
double values_LM358_SOUND = 0;
int cpt = 0;


// Here is the part of the program which will be executed once on start-up
void setup() 
{
  if (DEBUG)
    Serial.begin(9600);

  Serial.println("Setup");

  // Initializing Midir
  midir.begin(WIFI_SSID, WIFI_WPA_KEY, THINGSPEAK_SERVER, THINGSPEAK_API_KEY);

  // Iinit I2C sensor
  tsl2561.begin();
  htu21df.begin();

  // Init the connection to analog sensor (input or output entry)
  pinMode(PIN_HYSRF05_PRESENCE_ECHO, INPUT);
  pinMode(PIN_HYSRF05_PRESENCE_TRIGGER, OUTPUT);
  pinMode(PIN_LM358_SOUND, INPUT);


  /**********************************/
  /********* Your Code Here *********/
  /*********  INIT SENSORS  *********/

  // pinMode(MY_SENSOR, INPUT);

  /**********************************/
}



// Here is the part of the program which will be loop executed
void loop() 
{
  // Here we cumulate the values read on analog sensor on each loop (to do the average value then)
  values_HYSRF05_PRESENCE += read_HYSRF05_PRESENCE();
  values_LM358_SOUND += read_LM358_SOUND();


  /**********************************/
  /*******   Your Code Here   *******/
  /******* READ SENSORS VALUE *******/

  // my_sensor_value_total += analogRead(PIN_MY_SENSOR);
  // == OR with a method ==
  // my_sensor_value_total += my_sensor_read_method();

  /**********************************/

  // Counter of loops
  cpt++;
  
  if (cpt >= PUBLISH)
  {
    // Doing the average of values read by analog sensor
    double presence_HYSRF05 = values_HYSRF05_PRESENCE / cpt;
    double sound_LM358 = values_LM358_SOUND / cpt;

    // Getting the values from I2C digital sensors
    long light_TSL2561 = tsl2561.readVisibleLux();
    double humidity_HTU21DF = htu21df.readHumidity();
    double temperature_HTU21DF = htu21df.readTemperature();
    
    
    // Preparing the request
    midir.prepare();

    // Adding datas    
    midir.addData(FIELD_PRESENCE, "Presence", presence_HYSRF05);
    midir.addData(FIELD_SOUND, "Sound", sound_LM358);

    // 999 : HTU CRC_Check failed 
    // 998 : HTU is unplugged
    if (temperature_HTU21DF < 998)
      midir.addData(FIELD_TEMPERATURE, "Temperature", temperature_HTU21DF);

    if (humidity_HTU21DF < 998)
      midir.addData(FIELD_HUMIDITY, "Humidity", humidity_HTU21DF);

    // -1 : TSL256 is unplugged
    if (light_TSL2561 > -1)
      midir.addData(FIELD_LIGHT, "Light", light_TSL2561);


    /**********************************/
    /*******   Your Code Here   *******/
    /******* ADD VALUE TO MIDIR *******/

    // double my_sensor_value_average = my_sensor_value_total / cpt;
    
    // If you need to debug :
    // Serial.print("My sensor : ");
    // Serial.println(my_sensor_value_average);
    
    // midir.addData(FIELD_FOR_MY_SENSOR, "My Sensor", my_sensor_value_average);
    // my_sensor_value = 0;

    /**********************************/


    // Sending datas
    midir.send();

    // Re-initiliazing variables
    cpt = 0;
    values_HYSRF05_PRESENCE = 0;
    values_LM358_SOUND = 0;
  }

  // Pausing the program 10 milliseconds
  delay(TIME_SLEEP);
}




/*  FUNCTION read_HYSRF05_PRESENCE() : 
 *  - PARAMS : 0
 *  - RETURN : The distance in centimeter
 *  HOW DOES IT WORK :
 *  - Send a pulse signal to I/O TRIG which is at least 10us long, this will activate the module to start detecting
 *  - The presenceic module will automatically send eight 40khz square waves, and will automatically detect when there is a reflect signal
 *  - When there is an reflect signal back, the ECHO I/O will output a high level, the duration of the high_level signal is the time from untral sonic launch to return
 *  - As a result, the measured distance = (T(Time of High Level output ) * Speed of Sound(340M / S)) / 2
 */
long read_HYSRF05_PRESENCE(double temperature)
{
  /* Give a short LOW pulse beforehand to ensure a clean HIGH pulse */
  digitalWrite(PIN_HYSRF05_PRESENCE_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_HYSRF05_PRESENCE_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_HYSRF05_PRESENCE_TRIGGER, LOW);
  long duration = pulseIn(PIN_HYSRF05_PRESENCE_ECHO, HIGH, 50000); // Timeout of 50ms

  // Getting the speed of sound in cm/µs
  double speedSound = (331.3 + 0.606 * temperature) / 10000;

  long distanceCm = (duration * speedSound) / 2 ;
  //double distanceIn = ((duration * speedSound) / 2) / 2.54;

  return distanceCm;
}




/*  FUNCTION read_LM358_SOUND() : 
 *  - PARAMS : 0
 *  - RETURN : The value read by the sensor
 */

int peakToPeak = 0;
 
double read_LM358_SOUND()
{
  unsigned long startMillis = millis(); // Start of sample window

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < 50)
  {
    unsigned int sample = analogRead(PIN_LM358_SOUND);
    
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
        signalMax = sample;  // save just the max levels
      else if (sample < signalMin)
        signalMin = sample;  // save just the min levels
    }
  }

  if (signalMax - signalMin > 10)
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    
  double volts = peakToPeak * (ADC_VALUE / 1024);  // convert to mV

  double dB = 20 * log(volts/35);

  return dB;
}
