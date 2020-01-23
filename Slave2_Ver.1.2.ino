/* 
Author  : Dhiki Handika
Date    : 09/12/2019
*/

//==========================================================================================================================================//
//======================================================|    Initilization program   |======================================================//                                         
//==========================================================================================================================================//
#include <SoftwareSerial.h>                                                          // Add library SoftwareSerial
SoftwareSerial Monit (8,9);                                                          // Define Pin RX, TX SoftwareSerial Promini 
#define DETECTOR 3                                                                   // Define pin 4 to DETECTOR/SENSOR
#define ENABLE_RS485 2                                                               // Define pin 2 to ENABLE/DISABLE comunication data RX/TX

int32_t DataCount = 0;                                                                   // Define variable "DataCount" to store data counting                                                                           // Define variable "Count" to counting DETECTOR
int ReadSensorCurrent = 0; 
int ReadSensorLast = 0;

bool detector_read = false;
bool command_read = false;

//==========================================================================================================================================//
//================================================|    Procedure BUZZER and LAMP    |=======================================================//                                         
//==========================================================================================================================================//
void EnableTransmitter(){
    digitalWrite(ENABLE_RS485, HIGH);
}
void EnableReceiver(){
    digitalWrite(ENABLE_RS485, LOW);
}

//==========================================================================================================================================//
//=======================================================|    Setup program   |=============================================================//                                         
//==========================================================================================================================================//
void setup(){
  Serial.begin(9600);                                                               // Setting baud rate transmission data is "57600" b/s 
  Monit.begin(9600);                                                                // Setting baud rate Serial Monitor SoftwareSerial is "57600" b/s 
  pinMode(ENABLE_RS485, OUTPUT);                                                     // Setting up ENABLE_RS485 as a OUTPUT
  pinMode(DETECTOR, INPUT_PULLUP);                                                   // Settiing up DETECTEOR as a INPUT_PULLUP
}

//==========================================================================================================================================//
//=====================================================|    Main Loop program   |===========================================================//                                         
//==========================================================================================================================================//
void loop() {
  // Call procedure enble receiver RS485 communication
  EnableReceiver();
    /*Line debungging*/
  // int radd = digitalRead(ENABLE_RS485);
  // if(radd == LOW){
  //   Monit.println("Receiver ON");
  // } else {
  //   Monit.println("Transmitter ON");
  // }
  // Read Enable pin transmitter or receiver
  /* DETECTOR/Sensor Pehotoelectric or proximity has a behavior ACTIVE LOW*/
  ReadSensorCurrent = digitalRead(DETECTOR);
  if(ReadSensorCurrent == HIGH){
     ReadSensorLast = ReadSensorCurrent;
  } 
  // Serial.print(ReadSensorLast);                                                // Line debugging
  // Serial.println(ReadSensorCurrent);                                           // Line debugging
  if(ReadSensorLast != ReadSensorCurrent){
    detector_read = true;
    ReadSensorLast = ReadSensorCurrent;
  }

  if(Serial.available()){
    // int debugInt = Serial.read();
    // Monit.println(debugInt);                                                   // Line debugging
    EnableReceiver();
    String ReadData = Serial.readString();
    Monit.println(ReadData); 
    if(ReadData == "SENSOR_2"){
      command_read = true;
      // Call procedure enable transmitter communication data
    }
  }
  
  if(command_read == true){
    EnableTransmitter();
    Monit.print("Send counting data= ");                                                   // Line debugging
    Monit.println(DataCount);
    Serial.print("S");                                                          // Send serial counting data to Module Master
    Serial.print(DataCount);
    Serial.print("\n"); 
    delay(1000);
    command_read = false;
  }

  if(detector_read == true){
    DataCount = DataCount + 1;
    // Call procedure enable transmitter communication data
    //  EnableTransmitter();
    //  Serial.write(DataCount);                                                  // Send serial counting data to Module Master
    Monit.println(DataCount);                                                    // Line debugging
    detector_read = false;   
  } else{
     if(detector_read == false){
      // Call procedure enable receiver communication data
      //  EnableReceiver();
      //  Monit.println(DataCount);
     }
   }
}
