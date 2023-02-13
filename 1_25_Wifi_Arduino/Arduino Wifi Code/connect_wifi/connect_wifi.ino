
#include <WiFi.h>                  
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <avr/dtostrf.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <Arduino.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(52,5,226,201);  

//user and password refer to the username and password of the AWS database used to establish a connection

char user[] = "sql9596235";              
char password[] = "7LCYhZVIwK";  

//ssid and pass refer to the specifications of the WiFi netwok being connected to

char ssid[] = "407";    
char pass[] = "Joshistall";       

//WiFiClient is used to create a connection to the mySQL database

WiFiClient client;            
MySQL_Connection conn((Client *)&client);

//Beginning of the setup function

void setup() {
  
  Serial.begin(115200);   //Start the serial monitor

  while (!Serial);    //loop until the serial monitor is established

    Serial.println(F("BME280 test"));

    unsigned BMEstatus;
    
    // default settings
    BMEstatus = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!BMEstatus) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();

    float temp = bme.readTemperature();
    float press = bme.readPressure() / 100.0F;
    float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    float hum = bme.readHumidity();

    int status = WiFi.begin(ssid, pass);    //attempt to establish a WiFi connection at the given network

  if ( status != WL_CONNECTED) {

    Serial.println("Couldn't get a wifi connection");   //Tell the user that the system could not establish a connection
    while(true);                                        //loop until a connection is established

  }

  else {

    Serial.println("Connected to network");   //Tell user the system was able to connect to the internet
    IPAddress ip = WiFi.localIP();            //Retrieve the ip address of the system
    Serial.print("My IP address is: ");       //Print out the ip address to the serial monitor
    Serial.println(ip);                       //Print out the ip address to the serial monitor

  }

  IPAddress test;
  WiFi.hostByName("sql9.freemysqlhosting.net", test);
  Serial.println(test);

  Serial.println("Connecting to Database...");                //Inform user that the system is now trying to connect to the database

  char sqlQuery[] = "INSERT INTO sql9596235.sensors (temperature, pressure, altitude, humidity) VALUES (";
  char deleteQuery[] = "DELETE FROM sensors";

  char temperature[8];
  char pressure[8];
  char altitude[8];
  char humidity[8];
 
  dtostrf(temp, 6, 2, temperature);
  dtostrf(press, 6, 2, pressure);
  dtostrf(alt, 6, 2, altitude);
  dtostrf(hum, 6, 2, humidity);
 
  strcat(sqlQuery, temperature);
  strcat(sqlQuery, ",");
  strcat(sqlQuery, pressure);
  strcat(sqlQuery, ",");
  strcat(sqlQuery, altitude);
  strcat(sqlQuery, ",");
  strcat(sqlQuery, humidity);
  strcat(sqlQuery, ")");

  Serial.println(sqlQuery);

  if (conn.connect(server_addr, 3306, user, password)) {      //if the connection to the database at the specified location is true...

    delay(1000);                                              //delay for 1 second                         //print statement to check if it made it to this part of the loop
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);          //Create a mySQL cursor that will be used to access the database
    cur_mem->execute(deleteQuery);
    cur_mem->execute(sqlQuery);                               //point the cursor to execute the specified query
    delete cur_mem;                                           //delete cursor for memory purposes

  }

  else Serial.println("Connection failed.");                  //connection has failed if it reaches this else statement
    conn.close();                                                   //close the WiFi connection
}

void loop() {
    //printValues();
    //delay(delayTime);
}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}


