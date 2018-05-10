#include <Wire.h> // Must include Wire library for I2C
#include <SFE_MMA8452Q.h> // Includes the SFE_MMA8452Q library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define NUMPIXELS 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 200; // delay for half a second

MMA8452Q accel;

void setup()
{
  Serial.begin(9600);
  pixels.begin();
  Serial.println("MMA8452Q Test Code!");
  
  accel.init();
}

void loop()
{
  if (accel.available())
  {
    // First, use accel.read() to read the new variables:
    accel.read();

    Serial.println(); // Print new line every time.

//Animations:

//    swirl();
//    wave(accel.z,accel.x,accel.y);
    game();

  }
}

void wave(int x, int y, int z){

  int j=12;
  for(int i=0; i<12; i++){
    for (i; i <= 6; i++) {
      pixels.setPixelColor(i, pixels.Color(z,x,y));
      pixels.setPixelColor(j-i, pixels.Color(z,x,y));
      pixels.show();
      Serial.println(accel.x);
      Serial.println(accel.y);
      Serial.println(accel.z);
      delay(50); // speed
    }
  }
}

void swirl(){

 int r=0;
 int g=4;
 int b=8;

  for(int j=r; j<=0; j++){

    for (j; j < 4; j++) {
      pixels.setPixelColor(j, pixels.Color(accel.z,accel.x,accel.y));
      pixels.setPixelColor(j+g, pixels.Color(accel.y,accel.z,accel.x));
      pixels.setPixelColor(j+b, pixels.Color(accel.x,accel.y,accel.z));
      pixels.show();
      delay(40);
    }   
  }
}

void game(){

  for(int i = 0; i<10;i++){
   accel.read();
   randomSeed(analogRead(0));

   int RandX = random(-1000,1000);
   int RandY = random(-1000,1000);
   int RandZ = random(-1000,1000);

   Serial.println("X " + RandX);
   Serial.println("Y " + RandY);
   Serial.println("Z " + RandZ);


   for(int i = 0; i<3; i++){

    wave(RandZ,RandX,RandY);
    Serial.println("inside for" + i);
    delay(1000);
    wave(0,0,0);
  }  
  while (accel.z != RandZ){
    accel.read();
    
    wave(accel.z,accel.x,accel.y);
    Serial.println("inside while");
    Serial.println(accel.z);
    Serial.println(accel.x);
    Serial.println(accel.y);                        
    
    if(accel.z == RandZ && accel.x == RandX && accel.y == RandY){
      for(int i=0; i<5; i++){
        wave(RandZ,RandX,RandY);
        delay(500);
        wave(0,0,0);
      }
      break;
    }
  }
}

}  



