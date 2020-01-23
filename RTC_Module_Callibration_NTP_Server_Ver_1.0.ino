/* 
Author  : Dhiki Handika
Date    : 12/12/2019
*/

//==========================================================================================================================================//
//======================================================|    Initilization program   |======================================================//                                         
//==========================================================================================================================================//
#include <SoftwareSerial.h>                                                          // Add library SoftwareSerial
SoftwareSerial Monit(6,7);                                                           // Define Pin RX, TX SoftwareSerial Promini 

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>                                                       // Include Library can make ESP8266 interact with Web Server
#include <ArduinoJson.h>                                                             // Include Library Arduino Json  version 6.x.x
#include <NTPClient.h>                                                               // Include Library to acces NTP(Network Time Protocol) Server
#include <WiFiUdp.h>                                                                 // Include Library trasport layer UDP (User Datagram Protocol) to acces NTp
#include <RTClib.h>                                                                  // Include Library RTC(Real Time Clock)
#include <Wire.h>

RTC_DS1307 rtc;                                                                      // Setting IC RTC its used = "RTC DS1307"
// char t[32];

// Setup SSID and Password your WiFi network
const char *ssid     = "Parametrik RnD";
const char *password = "tabassam";

/*
 * Settiing your time localization here
 * yu can find Offset In second your local time in 
 * https://www.epochconverter.com/timezones
*/
const long utcOffsetInSeconds = 25200;                                               // set offset time GMT +7 (Jakarta, Indonesia)
// const long utcOffsetInSeconds = 32400;                                            // set offset time GMT +9 (Tokyo, Japan)

// Setting date to day
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Use transport layer UDP ("User Datagram Protocol") to get date time from NTP Server
WiFiUDP ntpUDP;

// Define NTP Client to get time
// NTPClient timeClient(ntpUDP);
// NTPClient timeClient(ntpUDP, "jp.pool.ntp.org", utcOffsetInSeconds);              // Setting timeclient "jp.pool.ntp.org" its Japan time
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);                 // Setting timeclient "id.pool.ntp.org" its Indonesian time

// Declare global variable here
String dataCount1 = " ";                                                              
String dataCount2 = " ";                                                              

String inputString1 = " ";                                                            
String inputString2 = " ";                                                            

int setData = 0;                                                                     

char incommingData;

bool stringComplete = false;


//====================================================================================================================================================================//
//------------------------------------------------------------------ POST json Data from ESP8266 --------------------------------------------------------------------//
//====================================================================================================================================================================//
//ATENTION!!! 
//POST json data we can choose library ArduinoJson version which will use (ArduinoJson Version 5.x.x or ArduinoJson 6.x.x)
//In my program I'am use Library ArduinoJson Version 6.x.x
   
//Check WiFi connection status
String id_machine1 = "machine1";
String id_machine2 = "machine2";
// int count = 12;                                                                   // Line debugging             
float length = 2020.3;                                                               // Line debugging

// POST data machine 1
void PostDataMachine1(){
  if (WiFi.status() == WL_CONNECTED) {                                               // Check WiFi connection status
    StaticJsonDocument<300> JSONbuffer;                                              // Declaring static json document memory 'JSONbuffer' pool in-place
    JsonObject JSONencoder = JSONbuffer.createNestedObject();                        // Creates an object and assigns it to the specified key. in program
                                                                                     // key its = 'JSONencoder'
    // Define attribute/coloumn database table                                                                              
    JSONencoder["id_machine"] = id_machine1;                                         // Json format = "id_machine"
    JSONencoder["count"] = dataCount1;                                               // Json format = "count":(value from variable "dataCount1"),
    JSONencoder["length"] = dataCount1.toFloat()*3.14*16;                            // Json format = "length":(value from variable "length")
 
    char JSONmessageBuffer[300];                                                     // Define variable char array 'JSONmessageBuffer' with maximum nember of bytes is 300
    serializeJson(JSONencoder, JSONmessageBuffer, sizeof(JSONmessageBuffer));        // Serializes a JsonDocument to create a minified JSON document
    Serial.println(JSONmessageBuffer);                                               // Print value from varible "JSONmessageDuffer"
 
    HTTPClient http;                                                                 // Declare object of class HTTPClient
 
    http.begin("http://monitor.parametrik.co.id:3500/post/datacount");               // Specify request destination
    http.addHeader("Content-Type", "application/json");                              // Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);                                     // Send the POST request
    String payload = http.getString();                                               // Get the response payload
  
    Serial.print("POST status: ");
    Serial.println(httpCode);                                                        // Print HTTP return code (data POST status)
    Serial.println(payload);                                                         // Print request response payload (format json)
    Serial.println(" ");                                                             // Print 
 
    http.end();                                                                      // Close connection
 
  } else {
    Serial.println("Error in WiFi connection");                                      // Print "Error in WiFi connection"
  }
  delay(5000); 
}

// POST data machine 2
void PostDataMachine2(){
  if (WiFi.status() == WL_CONNECTED) {                                               // Check WiFi connection status
    StaticJsonDocument<300> JSONbuffer;                                              // Declaring static json document memory 'JSONbuffer' pool in-place
    JsonObject JSONencoder = JSONbuffer.createNestedObject();                        // Creates an object and assigns it to the specified key. in program
                                                                                     // key its = 'JSONencoder'
    // Define attribute/coloumn database table                                                                              
    JSONencoder["id_machine"] = id_machine2;                                         // Json format = "id_machine"
    JSONencoder["count"] = dataCount2;                                               // Json format = "count":(value from variable "dataCount2"),
    JSONencoder["length"] = dataCount2.toFloat()*3.14*16;                                      // Json format = "length":(value from variable "dataLength2")
 
    char JSONmessageBuffer[300];                                                     // Define variable char array 'JSONmessageBuffer' with maximum nember of bytes is 300
    serializeJson(JSONencoder, JSONmessageBuffer, sizeof(JSONmessageBuffer));        // Serializes a JsonDocument to create a minified JSON document
    Serial.println(JSONmessageBuffer);                                               // Print value from varible "JSONmessageDuffer"
 
    HTTPClient http;                                                                 // Declare object of class HTTPClient
 
    http.begin("http://monitor.parametrik.co.id:3500/post/datacount");               // Specify request destination
    http.addHeader("Content-Type", "application/json");                              // Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);                                     // Send the POST request
    String payload = http.getString();                                               // Get the response payload
  
    Serial.print("POST status: ");
    Serial.println(httpCode);                                                        // Print HTTP return code (data POST status)
    Serial.println(payload);                                                         // Print request response payload (format json)
    Serial.println(" ");                                                             // Print 
 
    http.end();                                                                      // Close connection
 
  } else {
    Serial.println("Error in WiFi connection");                                      // Print "Error in WiFi connection"
  }
  delay(5000); 
}



//==========================================================================================================================================//
//===============================================|    Procedure Read Serial data   |=======================================================//                                         
//==========================================================================================================================================//
void EZread() {                                                   
  if(Serial.available()){
    stringComplete = false;
    // Read incoming data 
    incommingData = (char)Serial.read();                                          // Set variable "incommingData" as a function Serial.read
    Serial.print("incommingData= ");                                              // Line debugging
    Serial.println(incommingData);                                                // Line debugging
    if(incommingData == 'R'){                                                     // Structure condition if read incomming  = ('R')
      incommingData = 0;                                                          // Reset all variable char "incommingData"
      setData = 1;                                                                // Setting variable integer "setData" = 1
    } else{
      if(incommingData == 'S'){                                                   // Structure condition if read incomming  = ('S')
        incommingData = 0;                                                        // Reset all variable char "incommingData"
        setData = 2;                                                              // Setting variable integer "setData" = 2
      } 
    } 

    // Collect data to variable string
    if(stringComplete == false){                                                  // Structure condition if variablle bool "stringComplete" = false
      if(setData == 1){                                                           // Structure condition if variablle integer "setData" = 1
        if(incommingData == '\n'){                                                // Structure condition if read incomming  = ('\n')
          dataCount1.remove(0, dataCount1.length());                              // Remove all data in variable string "dataCount1" start from index 0 until last index
          dataCount1 += inputString1;                                             // Its operator "compound addition" equivalent to the expression x = x + y;
          inputString1.remove(0, inputString1.length());                          // Remove all data in variable string "inputString1" start from index 0 until last index 
          Serial.print("long data 1= ");                                          // Line debugging 
          Serial.print(dataCount1);                                               // Line debugging 
          Serial.println(" meter");                                               // Line debugging 
          PostDataMachine1();                                                     // POST data machine 1 to database 
          stringComplete = true;                                                  // Setting variable bool "stringComplete" = true
        } else {
          inputString1 += incommingData;                                          // Its operator "compound addition" equivalent to the expression x = x + y;
        }
      }
      if(setData == 2){                                                           // Structure condition if variablle integer "setData" = 1
        if(incommingData == '\n'){                                                // Structure condition if read incomming  = ('\n')
          dataCount2.remove(0, dataCount2.length());                              // Remove all data in variable string "dataCount2" start from index 0 until last index
          dataCount2 += inputString2;                                             // Its operator "compound addition" equivalent to the expression x = x + y;
          inputString2.remove(0, inputString2.length());                          // Remove all data in variable string "inputString2" start from index 0 until last index
          Serial.print("long data 2= ");                                          // Line debugging
          Serial.print(dataCount2);                                               // Line debugging
          Serial.println(" meter");                                               // Line debugging
          PostDataMachine2();                                                     // POST data machine 2 to database
          stringComplete = true;                                                  // Setting variable bool "stringComplete" = true
        } else {
          inputString2 += incommingData;                                          // Its operator "compound addition" equivalent to the expression x = x + y;
        }
      }
    }                                                                                                              
  }  
}


//==========================================================================================================================================//
//=======================================================|    Setup program   |=============================================================//                                         
//==========================================================================================================================================//
void setup(){
  Serial.begin(57600);
  Monit.begin(9600);
  Wire.begin();
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("Connected");                                                       // Print on serial monitor "Connected"
  Serial.println("IP Address");                                                      // Print on serial monitor "IP Address"
  Serial.println(WiFi.localIP());                                                    // Print on serial monitor character inside function WiFi.LocalIP();
  Serial.println("");                                                                // print on serial monitor

  timeClient.begin();
  timeClient.update();
  
  // Callibration RTC module with NTP Server
  rtc.begin();
  // rtc.adjust(DateTime(__DATE__, __TIME__));
  rtc.adjust(DateTime(2019, 8, 21, timeClient.getHours(), timeClient.getMinutes(), 
  timeClient.getSeconds()));
}


//==========================================================================================================================================//
//=========================================================|    Main Loop program   |=======================================================//                                         
//==========================================================================================================================================//
void loop() {
  timeClient.update();
  DateTime now = rtc.now();
  
  // Show time from NTP Server
  Serial.print("Time NTP Server= ");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());

  // delay(1000);
  Serial.print("Time moduel RTC= ");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  // Show time from RTC module
  Serial.print(", ");
  Serial.print(now.hour());       //Show hour 
  Serial.print(":");
  Serial.print(now.minute());     //Show Minute
  Serial.print(":");
  Serial.print(now.second());     //Show Second
  Serial.println();

  // Received data from Module Controller (Arduino MEGA 2560)
  EZread();

  // // Debbunging procedure POST data to database
  // PostDataMachine1();
  // PostDataMachine2();
  // delay(5000);
}
