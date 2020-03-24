/*
Author  : Redieta F.Z
Date    : 18/03/2020
*/

//==========================================================================================================================================//
//===================================================|   Initialization Program    |========================================================//                                         
//==========================================================================================================================================//
#include <SoftwareSerial.h>
SoftwareSerial SerialOne(10,11); //RX, TX
#define sensor 2

#define DEBUG
String inputString = "";         // a String to hold incoming data

int SensorNow = 0;
int count = 222;                 // set a dummy
const byte LED = 12;
bool sensor_read = false;
bool stringComplete = false;  // whether the string is complete

//==========================================================================================================================================//
//================================================|   Procedure proccesing counting    |====================================================//                                         
//==========================================================================================================================================// 
void active(){
  SensorNow = digitalRead(sensor);
  
  if(SensorNow == LOW){
    count = count+1;
    digitalWrite(LED, HIGH);
    sensor_read = true;
  }

  if(SensorNow == HIGH){
    digitalWrite(LED, LOW);
  }
}


//==========================================================================================================================================//
//========================================================|   Setup Program    |============================================================//                                         
//==========================================================================================================================================//
void setup() {
  Serial.begin(115200);
  SerialOne.begin(9600);
  inputString.reserve(150);
  pinMode(sensor, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), active, CHANGE);
}


//==========================================================================================================================================//
//======================================================|   Main Loop Program    |==========================================================//                                         
//==========================================================================================================================================//
void loop() {
  /* DBUG serial data counting */
  if(sensor_read == true){
    SerialOne.println(count);
    sensor_read = false;
  }
   if (stringComplete) {
    SerialOne.println(inputString);
    if(inputString == "S_2\n"){
        Serial.print("B");
        Serial.print(count);
        Serial.print("\n");
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}


//==========================================================================================================================================//
//==========================================================|   Serial ISR    |=============================================================//                                         
//==========================================================================================================================================//
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
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
