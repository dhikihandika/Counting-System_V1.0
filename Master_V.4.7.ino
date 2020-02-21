/*
Author  : Namira Fathani
Date    : 11/12/2019
*/


//==========================================================================================================================================//
//===================================================|   Initialization Program    |========================================================//                                         
//==========================================================================================================================================//
/* Add some library */
#include <LiquidCrystal.h>                                          // Add library LiquidCrystal
#include <RTClib.h>                                                 // Add library RTC
#include <Wire.h>                                                   // Add Library Wire RTC communication
// #include <Ethernet.h>                                               // Add library ethernet
// #include <SPI.h>                                                    // Add library protocol communication SPI
// #include <ArduinoJson.h>                                            // Add library arduino json 
// #include <PubSubClient.h>                                           // Add library PubSubClient MQTT

/* define some pin as I/O */
#define RS485 48                                                    // Define pin 48 as Enable RS485
#define BUTTON_1 2                                                  // Define pin 2 as button

/* configur etheret communication */
// byte mac[]  = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
// IPAddress ip(192, 168, 1, 41);                                      // IP ethernet shield assigned, in one class over the server
// IPAddress server(192, 168, 1, 99);                                  // IP LAN (Set ststic IP in PC/Server)

// EthernetClient ethClient;
// PubSubClient client(ethClient);

/* configure LCD and RTC */
const int rs = 50, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;          // Initilization LCD pin
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                          // Initilization pin Liquid crystal 
RTC_DS1307 RTC;                                                     // Define type RTC as RTC_DS1307 (must be suitable with hardware RTC will be used)

/* define variable use rising and falling edge Button */
int readButtonCurrent_1;
int readButtonLast_1;

/* define varibale float use to */
float ropelength;
float around = 3.14*16;                                             //circumference of pipe

unsigned long previousMillis = 0;                                   // previous millis for sensor 

/* global variable bool data sensor */
bool stringComplete = false;                                        // whether the string is complete
bool pushButton = false;                                           // user bool pushbutton
bool showLcd_S1 = false;
bool showLcd_S2 = false;

/* global variable id modul sensor */
String id_machine1 ="MAC01_01";
String id_machine2 ="MAC02_01";


/* global variable count modul sensor */
String incomingData = "";                                           // a String to hold incoming data
uint32_t data_S1 = 0; 
uint32_t data_S2 = 0; 
uint32_t data_S3 = 0; 
uint32_t data_S4 = 0; 
uint32_t data_S5 = 0; 
uint32_t data_S6 = 0; 
uint32_t data_S7 = 0; 
uint32_t data_S8 = 0; 
uint32_t data_S9 = 0; 
uint32_t data_S10 = 0;
uint32_t data_S11 = 0; 
uint32_t data_S12 = 0; 

/* global variable reset modul sensor */
uint32_t dataRST_S1 = 0;
uint32_t dataRST_S2 = 0;
uint32_t dataRST_S3 = 0; 
uint32_t dataRST_S4 = 0;
uint32_t dataRST_S5 = 0; 
uint32_t dataRST_S6 = 0; 
uint32_t dataRST_S7 = 0; 
uint32_t dataRST_S8 = 0; 
uint32_t dataRST_S9 = 0; 
uint32_t dataRST_S10 = 0;
uint32_t dataRST_S11 = 0; 
uint32_t dataRST_S12 = 0; 

/* global variable count publish */
uint32_t dataPub_S1 = 0;
uint32_t dataPub_S2 = 0;
uint32_t dataPub_S3 = 0;
uint32_t dataPub_S4 = 0;
uint32_t dataPub_S5 = 0;
uint32_t dataPub_S6 = 0;
uint32_t dataPub_S7 = 0;
uint32_t dataPub_S8 = 0;
uint32_t dataPub_S9 = 0;
uint32_t dataPub_S10 = 0;
uint32_t dataPub_S11 = 0;
uint32_t dataPub_S12 = 0;

/* global variable reset modul sensor */
bool reset_S1 = false;
bool reset_S2 = false;
bool reset_S3 = false;
bool reset_S4 = false;
bool reset_S5 = false;
bool reset_S6 = false;
bool reset_S7 = false;
bool reset_S8 = false;
bool reset_S9 = false;
bool reset_S10 = false;
bool reset_S11 = false;
bool reset_S12 = false;


// //==========================================================================================================================================//
// //=========================================================|   Procedure reconnect    |=====================================================//                                         
// //==========================================================================================================================================//
// void reconnect(){
//   while(!client.connected()){
//     Serial.print("Attemping MQTT connection...");
//     if(client.connect("ethernetClient")){
//       Serial.println("connected");

//       client.subscribe("PSI/countingbenang/tablet/setlimit");
//     }
//     else{
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println("try again 5 second");

//       delay(5000);
//     }
//   }
// }


// //==========================================================================================================================================//
// //============================================|   Procedure publish data to app server  |===================================================//                                         
// //==========================================================================================================================================//
// /* publish data sensor 1 */
// void publishData_S1(){
//   Serial.print(data_S1);                                                                // line debugging
//   Serial.print("|substraction|");                                                       // line debugging
//   Serial.println(dataRST_S1);                                                           // line debugging
//   // dataPub_S1 = data_S1 - dataRST_S1;                                                 // operation data if controlle setting as RESET counting
//   dataPub_S2 = data_S2;                                                                 // operation data   
//   Serial.print("data_publish= ");
//   Serial.println(String(dataPub_S1, DEC));                                              // convert to Int to String

// /* ArduinoJson create jsonDoc 
// Must be know its have a different function 
// if you use library ArduinoJson ver 5.x.x or 6.x.x
// -- in this program using library ArduinoJson ver 5.x.x
// */
// const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(7);

//  DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);                                             // Create jsonBuffer as buffer size is Dynamic
//   JsonObject& JSONencoder = jsonBuffer.createObject();                                  // Encode object in jsonBuffer

//   /* object jsonBuffer */
//   JSONencoder["id_controller"] = "CTR01";
//   JSONencoder["id_machine"] = "MAC01_01";
//   JSONencoder["clock"] = "2020-02-12 10:03";
//   JSONencoder["count"] = dataPub_S2;
//   JSONencoder["length"] = ropelength;
//   JSONencoder["status"] = "0";
//   JSONencoder["limit"] = "";

//   char JSONmessageBuffer[500];
//   JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

//   Serial.println("Sending message to MQTT topic...");
//   Serial.println(JSONmessageBuffer);

//   client.publish("PSI/countingbenang/datacollector/reportdata", JSONmessageBuffer);
//   if(client.publish("PSI/countingbenang/datacollector/reportdata", JSONmessageBuffer) == true){
//     Serial.println("Succes sending message");
//   } else {
//     Serial.println("Error Sending");
//   }

// }

// /* publish data sensor 2 */
// void publishData_S2(){
//   Serial.print(data_S2);                                                                // line debugging
//   Serial.print("|substraction|");                                                       // line debugging
//   Serial.println(dataRST_S2);                                                           // line debugging
//   // dataPub_S2 = data_S2 - dataRST_S2;                                                 // operation data if controlle setting as RESET counting                                                   
//   dataPub_S2 = data_S2;                                                                 // operation data  
//   Serial.print("data_publish= ");
//   Serial.println(String(dataPub_S2, DEC));                                              // convert to Int to String

// const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(7);

//  DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
//   JsonObject& JSONencoder = jsonBuffer.createObject();

//   JSONencoder["id_controller"] = "CTR01";
//   JSONencoder["id_machine"] = "MAC01_02";
//   JSONencoder["clock"] = "2020-02-12 10:03";
//   JSONencoder["count"] = dataPub_S2;
//   JSONencoder["length"] = ropelength;
//   JSONencoder["status"] = "0";
//   JSONencoder["limit"] = "";

//   char JSONmessageBuffer[500];
//   JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
//   Serial.println("Sending message to MQTT topic...");
//   Serial.println(JSONmessageBuffer);
//   client.publish("PSI/countingbenang/datacollector/reportdata", JSONmessageBuffer);
//   if(client.publish("PSI/countingbenang/datacollector/reportdata", JSONmessageBuffer) == true){
//     Serial.println("Succes sending message");
//   } else {
//     Serial.println("Error Sending");
//   }

// }

// /* publish data sensor 3 */
// void publishData_3(){

// }



//==========================================================================================================================================//
//===================================================|   Procedure show on LCD    |=========================================================//                                         
//==========================================================================================================================================//
void lcdShow(){
  if(showLcd_S1){
    if(pushButton){
        Serial.print("Button OK");
        delay(10000);                                                      // hold screen LCD
        Serial.println("Selesai");

        // Software reset program
        bool stringComplete = false;                                    // whether the string is complete
        bool pushButton = false;                                           // user bool pushbutton
        bool showLcd_S1 = false;
        bool showLcd_S2 = false;
    } else {
        lcd.clear();
        LcdPrintTime();
        lcd.setCursor(0,0);
        lcd.print("ID    : ");
        lcd.print(id_machine1);
        lcd.setCursor(0,1);
        lcd.print("Count : ");  
        lcd.print(data_S1);              
        lcd.setCursor(0,2);
        lcd.print("Length: ");
        lcd.print(ropelength);
        LcdPrintTime();
        showLcd_S1 = false;
      }
  } else{
    if(showLcd_S2){
      if(pushButton){
        Serial.print("Button OK");
        delay(10000);                                                      // hold screen LCD
        Serial.println("Selesai");

        // Software reset program
        bool stringComplete = false;                                    // whether the string is complete
        bool pushButton = false;                                           // user bool pushbutton
        bool showLcd_S1 = false;
        bool showLcd_S2 = false;
      } else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ID    : ");
          lcd.print(id_machine2);
          lcd.setCursor(0,1);
          lcd.print("Count : "); 
          lcd.print(data_S2);               
          lcd.setCursor(0,2);
          lcd.print("Length: ");
          lcd.print(ropelength);
          LcdPrintTime();
          showLcd_S2 = false;
      }
    }
  }
}


//==========================================================================================================================================//
//================================================|    Procedure lcd Get RealTimeClock   |==================================================//                                         
//==========================================================================================================================================//
void LcdPrintTime(){
  DateTime now = RTC.now();
  lcd.setCursor(0,3);
  lcd.print("clock : ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  lcd.print(" WIB");
}


//==========================================================================================================================================//
//=================================================|     Procedure Push Button        |=====================================================//                                         
//==========================================================================================================================================//
void buttonread(){
  readButtonCurrent_1 = digitalRead(BUTTON_1);
    if(readButtonCurrent_1 == HIGH){
      readButtonLast_1 = readButtonCurrent_1;  
    }
    if(readButtonCurrent_1 != readButtonLast_1){
      pushButton = true;
      readButtonLast_1 = readButtonCurrent_1;
    }
}


//==========================================================================================================================================//
//===========================================|    Procedure Enable Transmitter/Receiver    |================================================//                                         
//==========================================================================================================================================//
void EnableTransmitter(){
digitalWrite(RS485, HIGH);
}
void EnableReceiver(){
digitalWrite(RS485, LOW);
}  


//==========================================================================================================================================//
//=========================================|   Procedure sending command to modul sensor    |===============================================//                                         
//==========================================================================================================================================//
void sendCommandGet(){
  unsigned long currentMillis = millis();                     //times running
    if(currentMillis - previousMillis == 5000) {
      EnableTransmitter();
      Serial3.print("SENSOR_1\n");
      Serial.println("command sending slave_1 !!!");
      delay(100);                                             // delay timing receiver and transmitter RS485
      EnableReceiver();

    } else {
      if(currentMillis - previousMillis == 10000){
        previousMillis = currentMillis;
        EnableTransmitter();
        Serial3.print("SENSOR_2\n");
        Serial.println("command sending slave_2 !!!");
        delay(100);                                           // delay timing receiver and transmitter RS485
        EnableReceiver();
      }
    }
  }


//==========================================================================================================================================//
//====================================================|   Show data             |============================================================//                                         
//==========================================================================================================================================//
void showData(){
    if (stringComplete){
      Serial.print("incomming data= ");
      Serial.println(incomingData);
      int first_S1 = incomingData.indexOf('A');                                     // determine indexOf 'A'
      int first_S2 = incomingData.indexOf('B');                                     // determine indesOf 'B'
      int last = incomingData.lastIndexOf('/n');                                    // determine lastInndexOf '\n
      // Serial.print(first_S1);                                                    // Line debugginig to know value int data first
      // Serial.println(first_S2);                                                  // Line debugging

      if(first_S1 == 0){                                                            // condition indexOf 'A'
        //* Parse incoming data to particular variable */ 
        String datasensor1 = incomingData.substring(first_S1, last);                // substring 
        Serial.print("prefix_A= ");                                                 // line debuggiing
        Serial.println(datasensor1);
        datasensor1.remove(0,1);                                                    // remove header incomming data
        datasensor1.remove(datasensor1.length()-1, datasensor1.length() - 0);       // remove fotter incomming data (/n)
        data_S1 = datasensor1.toInt();                                              // covert string to integer datasensor1 and save to 'data_S1'
        Serial.print("data_S1= ");                                                  // Line debbuging
        Serial.println(data_S1);                                                    // Line debbuging
        ropelength = data_S1*around;                                                // Calculate ropelength

        showLcd_S1 = true;                                                          // make true boolean lcdShow
        // publish data to server
        // publishData_S1();
      } else {
        if(first_S2 == 0){                                                         // condition indexOf 'B'
          String datasensor2 = incomingData.substring(first_S2, last);
          Serial.print("prefix_B= ");                                               // line debuggiing
          Serial.println(datasensor2);
          datasensor2.remove(0,1);                                                  // remove header incomming data
          datasensor2.remove(datasensor2.length()-1, datasensor2.length() - 0);     // remove fotter incomming data (/n)
          data_S2 = datasensor2.toInt();                                            // covert string to integer datasensor2 and save to 'data_S2'
          Serial.print("data_S2= ");                                                // Line debbuging
          Serial.println(data_S2);                                                  // Line debbuging
          ropelength = data_S2*around;                                              // Calculate ropelength

          showLcd_S2 = true;                                                        // make true boolean lcdShow 
          // publish data to server
          // publishData_S1();
        }
      }
      incomingData ="";
      stringComplete = false;
  }
}


//==========================================================================================================================================//
//=======================================================|   Setup Program    |=============================================================//                                         
//==========================================================================================================================================//
void setup(){
  // Ethernet.begin(mac, ip);
  lcd.begin(20,4);

  // Callibration RTC module with NTP Server
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  // RTC.adjust(DateTime(2019, 8, 21, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
 
  // client.setServer(server, 1883);
  // client.setCallback(callback);

  // initialize serial baudrate
  Serial.begin(9600);                                           
  Serial2.begin(57600);
  Serial3.begin(9600);

  // reserve 200 bytes for the incomingData:
  incomingData.reserve(200);

  // detremine pinmode input or output
  pinMode(BUTTON_1, INPUT);
  pinMode(RS485, OUTPUT);
}


//==========================================================================================================================================//
//====================================================|   Main Loop Program    |============================================================//                                         
//==========================================================================================================================================//
void loop(){
  sendCommandGet();                                         // Proceedure send command
  showData();                                               // Proceddure show data on serial monitor debugging
  lcdShow();                                                // procedure show data on lcd
  buttonread();                                             // Procedure read button
}


//==========================================================================================================================================//
//====================================================|   ISR                  |============================================================//                                         
//==========================================================================================================================================//
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent3(){
  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read();
    // add it to the incomingData
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    
    incomingData += inChar;
    if(inChar == '\n'){ 
      stringComplete = true;
    }
  }
}
