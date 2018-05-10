#include "AirQuality.h"
#include "Arduino.h"

AirQuality airqualitysensor;
int current_quality =-1;

int pmPin = 8;
unsigned long starttime;
unsigned long duration;
float ratio = 0;
float concentration = 0;
unsigned long lowpulseoccupancy = 0;

void setup()
{
  Serial.print("Initiating...");

  Serial.begin(9600);

  airqualitysensor.init(14);

  pinMode(pmPin, INPUT);
  starttime = millis();

}



void loop()
{
   PMSensor(pmPin,30000);

  AQSensor();
 
}




void PMSensor(int pin, unsigned long sampletime_ms)
{
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

     // Serial.print(lowpulseoccupancy);
     // Serial.print(", ");
     // Serial.print(ratio);
     // Serial.print(", ");
     // Serial.println(concentration);

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100   

    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
  
      Serial.print("LPO: ");  
     Serial.println(lowpulseoccupancy);
     Serial.print("Ratio: ");
     Serial.println(ratio);
     Serial.print("Concentration: ");
     Serial.println(concentration);
      
      lowpulseoccupancy = 0;
      ratio = 0;
      concentration = 0;
      starttime = millis();
  }
}




void AQSensor()
{
  current_quality=airqualitysensor.slope();
    if (current_quality >= 0)// if a valid data returned.
    {
        if (current_quality==0)
            Serial.println("High pollution! Force signal active");
        else if (current_quality==1)
            Serial.println("High pollution!");
        else if (current_quality==2)
            Serial.println("Low pollution!");
        else if (current_quality ==3)
            Serial.println("Fresh air");
    }
}


ISR(TIMER1_OVF_vect)
{
  if(airqualitysensor.counter==61)//set 2 seconds as a detected duty
  {

      airqualitysensor.last_vol=airqualitysensor.first_vol;
      airqualitysensor.first_vol=analogRead(A0);
      airqualitysensor.counter=0;
      airqualitysensor.timer_index=1;
      PORTB=PORTB^0x20;
  }
  else
  {
    airqualitysensor.counter++;
  }
}


 



