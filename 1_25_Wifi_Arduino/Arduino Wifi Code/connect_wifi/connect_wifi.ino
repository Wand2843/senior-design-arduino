
#include <WiFi.h>                  
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <EthernetClient.h>
#include <Ethernet.h>

#include <Arduino.h>
#include <ArduinoJson.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(54,236,169,70);  

//user and password refer to the username and password of the AWS database used to establish a connection

char user[] = "admin";              
char password[] = "seniordesign7034#";  

//ssid and pass refer to the specifications of the WiFi netwok being connected to

char ssid[] = "LC_HOME";    
char pass[] = "Born to Run";       

//WiFiClient is used to create a connection to the mySQL database

WiFiClient client;            
MySQL_Connection conn((Client *)&client);

//sqlQuery is a string of the test query to ensure the database is functioning properly

char sqlQuery[] = "INSERT INTO sys.test (message) VALUES ('Hello')";

//Beginning of the setup function

void setup() {
  
  Serial.begin(115200);   //Start the serial monitor

  while (!Serial);    //loop until the serial monitor is established

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

  Serial.println("Connecting to Database...");                //Inform user that the system is now trying to connect to the database

  if (conn.connect(server_addr, 3306, user, password)) {      //if the connection to the database at the specified location is true...

    delay(1000);                                              //delay for 1 second
    Serial.println("RAHHHH");                                 //print statement to check if it made it to this part of the loop
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);          //Create a mySQL cursor that will be used to access the database
    cur_mem->execute(sqlQuery);                               //point the cursor to execute the specified query
    delete cur_mem;                                           //delete cursor for memory purposes

  }

  else Serial.println("Connection failed.");                  //connection has failed if it reaches this else statement
    Serial.println(getTime());
    conn.close();                                                   //close the WiFi connection
}

void loop() {


}


