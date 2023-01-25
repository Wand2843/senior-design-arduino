#include <Wire.h>

const int slaveAddress = 0x20; // Address of I2C slave device
float temperature, humidity, pressure, adcValue, accX, accY, accZ, magX, magY, magZ;

void setup() {
  Wire.begin(slaveAddress);
  Wire.onReceive(receiveData);
  Serial.begin(9600);
}

void loop() {
  // Do something with the data
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" Humidity: ");
  Serial.print(humidity);
  Serial.print(" Pressure: ");
  Serial.print(pressure);
  Serial.print(" ADC value: ");
  Serial.print(adcValue);
  Serial.print(" AccX: ");
  Serial.print(accX);
  Serial.print(" AccY: ");
  Serial.print(accY);
  Serial.print(" AccZ: ");
  Serial.print(accZ);
  Serial.print(" MagX: ");
  Serial.print(magX);
  Serial.print(" MagY: ");
  Serial.print(magY);
  Serial.print(" MagZ: ");
  Serial.println(magZ);
}

void receiveData(int byteCount) {
  int i = 0;
  while (Wire.available()) {
    temperature = Wire.read();
    humidity = Wire.read();
    pressure = Wire.read();
    adcValue = Wire.read();
    accX = Wire.read();
    accY = Wire.read();
    accZ = Wire.read();
    magX = Wire.read();
    magY = Wire.read();
    magZ = Wire.read();
  }
}
