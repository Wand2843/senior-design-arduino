#include <Wire.h>

const int slaveAddress = 0x20; // Address of I2C slave device
float temperature, humidity, Hour, Minute, Second;

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
  Serial.print(" Hour: ");
  Serial.print(Hour);
  Serial.print(" Minute: ");
  Serial.print(Minute);
  Serial.print(" Second: ");
  Serial.print(Second);

}

void receiveData(int byteCount) {
  while (Wire.available()) {
    temperature = Wire.read();
    humidity = Wire.read();
    Hour = Wire.read();
    Minute = Wire.read();
    Second = Wire.read();
  }
}
