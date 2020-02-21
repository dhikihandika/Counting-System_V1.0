/* 
Author  : Dhiki Handika
Date    : 09/12/2019
*/

//==========================================================================================================================================//
//======================================================|    Initilization program   |======================================================//                                         
//==========================================================================================================================================//
#include <SoftwareSerial.h>                                                          // Add library SoftwareSerial
SoftwareSerial Monit (10,11);                                                          // Define Pin RX, TX SoftwareSerial Promini 
#define INDICATOR_DATA 4                                                             // Define pin 4 to INDICATOR DATA
#define DETECTOR 3                                                                   // Define pin 3 to DETECTOR/SENSOR 1
#define ENABLE_RS485 2                                                               // Define pin 2 to ENABLE/DISABLE comunication data RX/TX

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
uint32_t DataCount = 0;                                                               // Define variable "DataCount" to store data counting                                                                           // Define variable "Count" to counting DETECTOR
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
//====================================================|    Procedure to reset count   |=====================================================//                                         
//==========================================================================================================================================//
void DetResetCMD(){
    /* Read serial data reset */
  if(Serial.available()){
    // int debugInt = Serial.read();                                             // Line debugging
    // Monit.println(debugInt);                                                  // Line debugging
    EnableReceiver();
    String ReadData = Serial.readString();
    Monit.println(ReadData); 
    if(ReadData == "S1_RST"){
    DataCount = 0;                                                               // Reset data counting
    Monit.print("start data counting= ");
    Monit.println(DataCount);
      // blink led 5x
      for(int n = 0; n<= 5; n++){
        IndicatorDataOn();
        delay(300);
        IndicatorDataOff();
        delay(300);
      }
    }
  }
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
  inputString.reserve(200);
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

  //Serial data received processing
  if (stringComplete) {
    Monit.println(inputString);
    if(inputString == "SENSOR_1\n"){
      command_read = true;
      Monit.println(inputString); 
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  /* Send data to modul master */
  if(command_read == true){
    delay(200);
    EnableTransmitter();
    Monit.print("Send counting data sensor 1= ");                                         // Line debugging
    Monit.println(DataCount);
      Serial.print("A");                                                         // Send serial counting data to Module Master
      Serial.print(DataCount);                                                   // Send serial counting data to Module Master
      Serial.print("\n");                                                        // Send serial counting data to Module Master
      delay(100);    
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
    IndicatorDataOn();                                                           // Indicator data HIGH
  } else {
    IndicatorDataOff();                                                          // Indicator data LOW
  }

  /* Reset Data Count */
  // DetResetCMD();
   
}

void serialEvent() {
/* Read serial data request */
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
