#define PIN_LM358_SOUND A2

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
unsigned long peakToPeak = 0;

long cpt = 0;
double avDb = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LM358_SOUND, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long startMillis = millis(); // Start of sample window

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(PIN_LM358_SOUND);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }

  if (signalMax - signalMin > 10)
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    
  double volts = peakToPeak * (5000 / 1024);  // convert to mV

  double dB = 20 * log(volts/30);

  return dB;
}