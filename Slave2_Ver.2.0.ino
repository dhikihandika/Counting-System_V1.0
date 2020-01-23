/* 
Author  : Dhiki Handika
Date    : 09/12/2019
*/

//==========================================================================================================================================//
//======================================================|    Initilization program   |======================================================//                                         
//==========================================================================================================================================//
#include <SoftwareSerial.h>                                                          // Add library SoftwareSerial
SoftwareSerial Monit (8,9);                                                          // Define Pin RX, TX SoftwareSerial Promini 
#define INDICATOR_DATA 4                                                             // Define pin 4 to INDICATOR DATA
#define DETECTOR 3                                                                   // Define pin 3 to DETECTOR/SENSOR 1
#define ENABLE_RS485 2                                                               // Define pin 2 to ENABLE/DISABLE comunication data RX/TX


int32_t DataCount = 0;                                                               // Define variable "DataCount" to store data counting                                                                           // Define variable "Count" to counting DETECTOR
int ReadSensorCurrent = 0; 
int ReadSensorLast = 0;

bool detector_read = false;
bool indicator_data = false;
bool command_read = false;

//==========================================================================================================================================//
//========================================|    Procedure Enable RS485 and LAMP INDICATOR DATA   |===========================================//                                         
//==========================================================================================================================================//
/* Procedure Enable RS485 */
void EnableTransmitter(){
    digitalWrite(ENABLE_RS485, HIGH);
}
void EnableReceiver(){
    digitalWrite(ENABLE_RS485, LOW);
}

/* Procedure Lamp Indicator Data*/
void IndicatorDataOn(){
    digitalWrite(INDICATOR_DATA, HIGH);
}
void IndicatorDataOff(){
    digitalWrite(INDICATOR_DATA, LOW);
}

//==========================================================================================================================================//
//=======================================================|    Setup program   |=============================================================//                                         
//==========================================================================================================================================//
void setup(){
  Serial.begin(9600);                                                               // Setting baud rate transmission data is "57600" b/s 
  Monit.begin(9600);                                                                // Setting baud rate Serial Monitor SoftwareSerial is "57600" b/s 
  pinMode(INDICATOR_DATA, OUTPUT);                                                  // Setting up INDICATOR_DATA as OUTPUT
  pinMode(ENABLE_RS485, OUTPUT);                                                    // Setting up ENABLE_RS485 as a OUTPUT
  pinMode(DETECTOR, INPUT_PULLUP);                                                  // Settiing up DETECTEOR as a INPUT_PULLUP   
}

//==========================================================================================================================================//
//=====================================================|    Main Loop program   |===========================================================//                                         
//==========================================================================================================================================//
void loop() {
  // Call procedure enble receiver RS485 communication
  EnableReceiver();
  // Read Enable pin transmitter or receiver
  /* DETECTOR/Sensor Pehotoelectric or proximity has a behavior ACTIVE LOW
  *  In below this is schema program using Rising Edge and Falling Edge signal detector
  */
  ReadSensorCurrent = digitalRead(DETECTOR);
  if(ReadSensorCurrent == HIGH){
     indicator_data = false;
     ReadSensorLast = ReadSensorCurrent;
  } 
  // Serial.print(ReadSensorLast);                                                // Line debugging
  // Serial.println(ReadSensorCurrent);                                           // Line debugging
  if(ReadSensorLast != ReadSensorCurrent){
    detector_read = true;
    indicator_data = true;
    ReadSensorLast = ReadSensorCurrent;
  }

/* Read Serial data */
  if(Serial.available()){
    // int debugInt = Serial.read();                                             // Line debugging
    // Monit.println(debugInt);                                                  // Line debugging
    EnableReceiver();
    String ReadData = Serial.readString();
    Monit.println(ReadData); 
    if(ReadData == "SENSOR_2"){
    command_read = true;
    }
  }
  
  /* Send data to modul master */
  if(command_read == true){
    EnableTransmitter();
    Monit.print("Send counting data= ");                                         // Line debugging
    Monit.println(DataCount);
    Serial.print("S");                                                           // Send serial counting data to Module Master
    Serial.print(DataCount);                                                     // Send serial counting data to Module Master
    Serial.print("\n");                                                          // Send serial counting data to Module Master
    delay(1000);
    command_read = false;
  }

  /* Proccessing data counting */
  if(detector_read == true){
    DataCount = DataCount + 1;                                                   // Increamnet data counting + 1
    Monit.print("Counting= ");                                                   // Line debugging
    Monit.println(DataCount);  
    detector_read = false;
  } else{
  }

  /* Lamp Indicator data */
  if(indicator_data == true){
    IndicatorDataOn();                                                          // Indicator data HIGH
  } else {
    IndicatorDataOff();                                                         // Indicator data LOW
  }
   
}
