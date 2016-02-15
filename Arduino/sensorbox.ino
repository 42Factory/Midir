/* Importing needed librairies for I2C digital sensor */
#include "Digital_Light_TSL2561.h"
#include "HTU21DF.h"

#define HOSTNAME "sensorbox-1"  /* Sensorbox ID will appear on the firebase */
#define TIME_SLEEP 10           /* Time in ms between each loop */
#define PUBLISH 10000             /* Number of loop between each transmission of datas */

/* Here we define PINS on which analog sensor are connected to */
#define PIN_HYSRF05_ULTRASON_ECHO 5
#define PIN_HYSRF05_ULTRASON_TRIGGER 6
#define PIN_LM358_SOUND A2

/* Declaring variables */
long values_HYSRF05_ULTRASON = 0;
long values_LM358_SOUND = 0;
int cpt = 0;

/* Here is the part of the program which will be executed once on start-up */
void setup() 
{
  /* Opening USB Serial connection with the raspberry */
  Serial.begin(9600);

  /* Iinit I2C sensor */
  tsl2561.begin();
  htu21df.begin();

  /* Init the connection to analog sensor (input or output entry) */
  pinMode(PIN_HYSRF05_ULTRASON_ECHO, INPUT);
  pinMode(PIN_HYSRF05_ULTRASON_TRIGGER, OUTPUT);
  pinMode(PIN_LM358_SOUND, INPUT);

  /* Sending initialise settings to the raspberry */
  Serial.println("SETUP");
  Serial.print("HOSTNAME ");
  Serial.println(HOSTNAME);
}

/* Here is the part of the program which will be loop executed */
void loop() 
{
  /* Here we cumulate the values read on analog sensor on each loop (to do the average value then) */
  values_HYSRF05_ULTRASON += read_HYSRF05_ULTRASON();
  values_LM358_SOUND += read_LM358_SOUND();

  /* Counter of loops */
  cpt++;
  
  if (cpt >= PUBLISH)
  {
    /* Doing the average of values read by analog sensor  */
    double distance_HYSRF05 = values_HYSRF05_ULTRASON / cpt;
    double sound_LM358 = values_LM358_SOUND / cpt;

    /* Getting the values from I2C digital sensors */
    long lux_TSL2561 = tsl2561.readVisibleLux();
    double humidity_HTU21DF = htu21df.readHumidity();
    double temperature_HTU21DF = htu21df.readTemperature();

    /* Telling to raspberry it's time to record */
    Serial.println("BEGIN");

    /* Sending datas */
    Serial.print("DISTANCE_ULTRASON ");
    Serial.println(distance_HYSRF05);
    
    Serial.print("SOUND ");
    Serial.println(sound_LM358);

    /* -1 : TSL256 is unplugged */
    if (lux_TSL2561 > -1)
    {
      Serial.print("LUX ");
      Serial.println(lux_TSL2561);
    }

    /* 999 : HTU CRC_Check failed 
     * 998 : HTU is unplugged */
    if (humidity_HTU21DF < 998)
    {
      Serial.print("HUMIDITY ");
      Serial.println(humidity_HTU21DF);
    }

    if (temperature_HTU21DF < 998)
    {
      Serial.print("TEMPERATURE ");
      Serial.println(temperature_HTU21DF);
    }

    /* Telling to raspberry it's the ending of transmission */
    Serial.println("END");

    /* Re-initiliazing variables */
    cpt = 0;
    values_HYSRF05_ULTRASON = 0;
    values_LM358_SOUND = 0;
  }

  /* Pausing the program 10 milliseconds */
  delay(TIME_SLEEP);
}

/*  FUNCTION read_HYSRF05_ULTRASON() : 
 *  - PARAMS : 0
 *  - RETURN : The distance in centimeter
 *  HOW DOES IT WORK :
 *  - Send a pulse signal to I/O TRIG which is at least 10us long, this will activate the module to start detecting
 *  - The ultrasonic module will automatically send eight 40khz square waves, and will automatically detect when there is a reflect signal
 *  - When there is an reflect signal back, the ECHO I/O will output a high level, the duration of the high_level signal is the time from untral sonic launch to return
 *  - As a result, the measured distance = (T(Time of High Level output ) * Speed of Sound(340M / S)) / 2
 */
long read_HYSRF05_ULTRASON()
{
  /* Give a short LOW pulse beforehand to ensure a clean HIGH pulse */
  digitalWrite(PIN_HYSRF05_ULTRASON_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_HYSRF05_ULTRASON_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_HYSRF05_ULTRASON_TRIGGER, LOW);
  long duration = pulseIn(PIN_HYSRF05_ULTRASON_ECHO, HIGH, 50000); // Timeout of 50ms
 
  /* Convert the time into a distance */
  long distanceCm = duration / 29.1 / 2 ;
  //double distanceIn = duration / 74 / 2;

  return distanceCm;
}

/*  FUNCTION read_LM358_SOUND() : 
 *  - PARAMS : 0
 *  - RETURN : The value read by the sensor
 */
long read_LM358_SOUND()
{
  return analogRead(PIN_LM358_SOUND);
}


