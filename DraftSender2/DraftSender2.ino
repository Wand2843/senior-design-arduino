#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

const int slaveAddress = 0x20; // Address of I2C slave device

unsigned status;
  
  // default settings
void setup() {
  Wire.begin();
  bme.begin(); // Initialize BME280 sensor

}

void loop() {
  tmElements_t tm;

  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;


  // Send data to slave device
  Wire.beginTransmission(slaveAddress);
  Wire.write(int(temperature)); // Send temperature data
  Wire.write(int(humidity)); // Send humidity data
  Wire.write(tm.Hour); // Send hour data
  Wire.write(tm.Minute); // Send minute data
  Wire.write(tm.Second); // Send second data
  


  Wire.endTransmission();

  delay(1000); // Wait for a second before reading again
}
