#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>


Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);
float averageH=0;
int i=0;
int OneP=0; 
int TwoP=0; 
int ThreeP=0;
 int FourP=0;
  int FiveP=0;
   int OneN=0; 
   int TwoN=0; 
   int ThreeN=0;
    int FourN=0;
     int FiveN=0;
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Magnetometer Test"); Serial.println("");

  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There wasa problem detecting the LIS2MDL ... check your connections */
    Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!");
    while(1);
  }
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  float offsety= 65+event.magnetic.y;
  float offsetx= 60+event.magnetic.x;
  float heading = ((atan2(offsety,offsetx)) * 180) / Pi;
  /*Serial.print (event.magnetic.y);
  Serial.print(" ");
Serial.print (event.magnetic.x);
  Serial.print(" ");
  */
  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
i++;
averageH=averageH+heading;
float average= averageH/i;
float desiredA=301;

/*if(300.00>=299.99&&300.00<(300.00+1.00)){
  Serial.println("W");
}
else {
  Serial.println("F");
}
*/
int inc=0;
if(heading>=desiredA&& heading<(desiredA+1.00)){
  OneP++;
  inc=1;
}
if(heading>=(desiredA+1.00)&& heading<(desiredA+2.00)){
  TwoP++;
  inc=2;
}
if(heading>=(desiredA+2.00)&& heading<(desiredA+3.00)){
  ThreeP++;
  inc=3;
}
if(heading>=(desiredA+3.00)&& heading<(desiredA+4.00)){
  FourP++;
  inc=4;
}
if(heading>=(desiredA+4.00)){
  FiveP++;
  inc=5;
}
if(heading<=desiredA&& heading>(desiredA-1.00)){
  OneN++;
  inc=6;
}
if(heading<=(desiredA-1.00)&& heading>(desiredA-2.00)){
  TwoN++;
  inc=7;
}
if(heading<=(desiredA-2.00)&& heading>(desiredA-3.00)){
  ThreeN++;
  inc=8;
}
if(heading<=(desiredA-3.00)&& heading>(desiredA-4.00)){
  FourN++;
  inc=9;
}
if(heading<=(desiredA-4.00)){
  FiveN++;
  inc=10;
}
/*Serial.print(inc);
Serial.print(" ");
Serial.println(heading);

Serial.print(desiredA-4.00);
Serial.print(" ");
if(296.3<=(desiredA-4.00)){
  Serial.println("A");
}
else {
  Serial.println("B");
}
*/
  Serial.print("Compass Heading: ");
  Serial.print(heading);
Serial.print(" ");
Serial.print(i);
Serial.print(" ");
Serial.println(average);
/*
Serial.print(" ");
Serial.print(FiveP);
Serial.print(" ");
Serial.print(FourP);
Serial.print(" ");
Serial.print(ThreeP);
Serial.print(" ");
Serial.print(TwoP);
Serial.print(" ");
Serial.print(OneP);
Serial.print(" ");
Serial.print(OneN);
Serial.print(" ");
Serial.print(TwoN);
Serial.print(" ");
Serial.print(ThreeN);
Serial.print(" ");
Serial.print(FourN);
Serial.print(" ");
Serial.println(FiveN);
*/
delay(1000);
}
