#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_LSM303.h>

const int bme280Address = 0x76; // Address of BME280 sensor
const int adcAddress = 0x48; // Address of Adafruit 3296
const int lsm303Address = 0x1D; // Address of LSM303 sensor
const int slaveAddress = 0x20; // Address of I2C slave device
Adafruit_BME280 bme; // BME280 sensor object
Adafruit_ADS1115 adc(adcAddress); // Adafruit 3296 object
Adafruit_LSM303 lsm; // LSM303 sensor object

void setup() {
  Wire.begin();
  bme.begin(bme280Address); // Initialize BME280 sensor
  adc.begin(); // Initialize Adafruit 3296
  lsm.begin(); // Initialize LSM303 sensor
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  int16_t adcValue = adc.readADC_SingleEnded(0); // Read from A0 channel
  // Read data from LSM303
  sensors_event_t event;
  lsm.getEvent(&event);
  float accX = event.acceleration.x;
  float accY = event.acceleration.y;
  float accZ = event.acceleration.z;
  float magX = event.magnetic.x;
  float magY = event.magnetic.y;
  float magZ = event.magnetic.z;

  // Send data to slave device
  Wire.beginTransmission(slaveAddress);
  Wire.write(temperature); // Send temperature data
  Wire.write(humidity); // Send humidity data
  Wire.write(pressure); // Send pressure data
  Wire.write(adcValue); // Send adcValue data
  Wire.write(accX); // Send accX data
  Wire.write(accY); // Send accY data
  Wire.write(accZ); // Send accZ data
  Wire.write(magX); // Send magX data
  Wire.write(magY); // Send magY data
  Wire.write(magZ); // Send magZ data


  Wire.endTransmission();

  delay(1000); // Wait for a second before reading again
}
