// Function: If the sound sensor senses a sound that is up to the threshold you set in the code, the LED is on for 200ms.
// Hardware: Grove - Sound Sensor, Grove - LED

/*macro definitions of the sound sensor and the LED*/
#define SOUND_SENSOR A0

int sensorValue = 0;
int cpt = 0;

void setup() 
{
    Serial.begin(9600);

    pinMode(SOUND_SENSOR, INPUT); 
}
 
void loop() 
{
  sensorValue += analogRead(SOUND_SENSOR);
  
  cpt++;
  
  if (cpt >= 100)
  {
    float averageValue = sensorValue / cpt;

    Serial.print("Sound value : ");
    Serial.println(averageValue);

    cpt = 0;
    sensorValue = 0;
  }
  
  delay(10);
}
