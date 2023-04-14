#include <math.h>

#define LATITUDE 40

// float julianDay = 14;

int year = 2024;
int month = 3;
int day = 14;
int hour = 5;
int minute = 30;

void setup() {
  // put your setup code here, to run once:
  delay(5000);
  float solarDeclination = getSolarDec(getJD(year,month,day));
  Serial.println(getJD(year,month,day));
  Serial.println();
  for (hour = 5;hour<20;hour++){
    minute=0;
    Serial.println(getJD(year,month,day));
    Serial.println(getAlt(LATITUDE,solarDeclination,getTimeP(hour,minute)));
    Serial.println(getAzi(LATITUDE,solarDeclination,getTimeP(hour,minute),getAlt(LATITUDE,solarDeclination,getTimeP(hour,minute))));
    Serial.println();
    minute=30;
    Serial.println(getAlt(LATITUDE,solarDeclination,getTimeP(hour,minute)));
    Serial.println(getAzi(LATITUDE,solarDeclination,getTimeP(hour,minute),getAlt(LATITUDE,solarDeclination,getTimeP(hour,minute))));
    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // float solarDeclination = getSolarDec(getJD(year,month,day));
  // Serial.println(getJD(year,month,day));
  // Serial.println(getAlt(LATITUDE,solarDeclination,getTimeP(hour,minute)));
  // Serial.println(getAzi(LATITUDE,solarDeclination,getTimeP(hour,minute),getAlt(LATITUDE,solarDeclination,getTimeP(hour,minute))));
  // Serial.println();
  // delay(5000);
}

float getAlt(float lat, float sd,float timeP){
  float hourAngle = getHourAngle(timeP);
  return degrees(asin(cos(radians(lat))*cos(radians(sd))*cos(radians(hourAngle))+sin(radians(lat))*sin(radians(sd))));
}

float getAzi(float lat, float sd,float timeP,float alt){
  float hourAngle = getHourAngle(timeP);
  float azi2;
  if(timeP<12){
    azi2 =(180.0-degrees(asin(cos(radians(sd))*sin(radians(hourAngle))/cos(radians(alt)))));
  }else{
    azi2 =(180.0-degrees(asin(cos(radians(sd))*sin(radians(hourAngle))/cos(radians(alt))))-360.0);
  }
  if(cos(radians(hourAngle))>=((tan(radians(sd)))/(tan(radians(lat))))){
    return (degrees(asin(cos(radians(sd))*sin(radians(hourAngle))/cos(radians(alt)))));
  }else{
    return azi2;
  }
}

float getHourAngle(float timeInput){
  return (12-timeInput)*15;
}

float getSolarDec(float jD){
  return 23.45*sin(radians(360.0/365*(jD-81)));
}

float getTimeP(int h, int m){
  return (float)h+((float)m)/60;
}

float getJD(int y, int m, int d){
  if(y%400!=0){
    switch(m){
      case 1:
      return d;
      break;
      case 2:
      return 31+d;
      break;
      case 3:
      return 59+d;
      break; 
      case 4:
      return 90+d;
      break;
      case 5:
      return 120+d;
      break;
      case 6:
      return 151+d;
      break;
      case 7:
      return 181+d;
      break;
      case 8:
      return 212+d;
      break;
      case 9:
      return 243+d;
      break;
      case 10:
      return 273+d;
      break;
      case 11:
      return 304+d;
      break;
      case 12:
      return 334+d;
      break;
    }
  }else{
    switch(m){
      case 1:
      return d;
      break;
      case 2:
      return 31+d;
      break;
      case 3:
      return 60+d;
      break; 
      case 4:
      return 91+d;
      break;
      case 5:
      return 121+d;
      break;
      case 6:
      return 152+d;
      break;
      case 7:
      return 182+d;
      break;
      case 8:
      return 213+d;
      break;
      case 9:
      return 244+d;
      break;
      case 10:
      return 274+d;
      break;
      case 11:
      return 305+d;
      break;
      case 12:
      return 335+d;
      break;
    }
  }
}
// float radians(float degrees) {
//   return degrees / 360 * 2 * pi;
// }