/*
Author  : dhikihandika
Date    : 18/03/2020
*/

//==========================================================================================================================================//
//===================================================|   Initialization Program    |========================================================//                                         
//==========================================================================================================================================//
#define DEBUG

#define timer1 5000                                 // timer send command to sensor module 1
#define timer2 10000                                // timer send command to sensor module 2


/* variable timer millis */
unsigned long currentMillis = 0;  
unsigned long currentMillis_errorData = 0;                   
unsigned long previousMillis = 0;

/* variable incoming data */
uint32_t data_S1 = 0;
uint32_t data_S2 =0;

/* variable status sensor */
int status_S1 = 0;
int status_S2 = 0;

/* varibale indexOf data */
int first = 0;
int last = 0;

/* varibale check status data */
int errorCheck_S1 = 0;
int errorCheck_S2 = 0;

String incomingData = "";                           // a String to hold incoming data
bool stringComplete = false;                        // whether the string is complete

bool prefix_A = false;
bool prefix_B = false;

//==========================================================================================================================================//
//=========================================================|   Setup Program    |===========================================================//                                         
//==========================================================================================================================================//
void sendCommand(){
    currentMillis = millis();
    if(currentMillis - previousMillis == timer1){
        #ifdef DEBUG
        Serial.println("---------------------------------------------------------------");  
        Serial.println("Send command to sensor module 1");
        Serial.println("");
        #endif // DEBUG
        Serial3.print("S_1\n");
    } else {
        if(currentMillis - previousMillis == timer2){
            previousMillis = currentMillis;
            #ifdef DEBUG
            Serial.println("---------------------------------------------------------------");  
            Serial.println("Send command to sensor module 2");
            Serial.println("");
            #endif // DEBUG
            Serial3.print("S_2\n");
        }
    }
}

//==========================================================================================================================================//
//======================================================|   Procedure to showDaota    |=====================================================//                                         
//==========================================================================================================================================//
void showData(){
  /* Show data for sensor 1 */
  if(prefix_A){
    if(stringComplete){
      status_S1 = 0;
      first = incomingData.indexOf('A');                                         // determine indexOf 'A'
      last = incomingData.lastIndexOf('/n');                                     // determine lastInndexOf '\n
      /* When true value is 0 and false is "-1" */

      /* Parse incoming data to particular variable */ 
      String datasensor1 = incomingData.substring(first, last);                  // substring 
      datasensor1.remove(0,1);                                                   // remove header incomming data
      datasensor1.remove(datasensor1.length()-1, datasensor1.length() - 0);      // remove fotter incomming data (/n)
      data_S1 = datasensor1.toInt();                                             // covert string to integer datasensor1 and save to 'data_S1'

      stringComplete = false;
      prefix_A = false;
      incomingData = "";

      #ifdef DEBUG
      Serial.println("Prefix_A --OK--");
      Serial.print("incomming data= ");Serial.print(incomingData);
      Serial.print(" | prefix= ");Serial.print(datasensor1);
      Serial.print(" | data S1= ");Serial.println(data_S1); 
      Serial.print("status S1= ");Serial.println(status_S1); 
      Serial.println("------------------------------||-------------------------------\n");                                              
      #endif //DEBUG
      } 
  } else {
    /* Show data for sensor 2 */
    if(prefix_B){
      if(stringComplete){
      status_S2 = 0;
      first = incomingData.indexOf('B');                                         // determine indexOf 'A'
      last = incomingData.lastIndexOf('/n');                                     // determine lastInndexOf '\n
      /* When true value is 0 and false is "-1" */

      /* Parse incoming data to particular variable */ 
      String datasensor2 = incomingData.substring(first, last);                  // substring 
      datasensor2.remove(0,1);                                                   // remove header incomming data
      datasensor2.remove(datasensor2.length()-1, datasensor2.length() - 0);      // remove fotter incomming data (/n)
      data_S2 = datasensor2.toInt();                                             // covert string to integer datasensor1 and save to 'data_S1'

      stringComplete = false;
      prefix_B = false;
      incomingData = "";

      #ifdef DEBUG
      Serial.println("Prefix_B --OK--");
      Serial.print("incomming data= ");Serial.print(incomingData);
      Serial.print(" | prefix= ");Serial.print(datasensor2);
      Serial.print(" | data_S2= ");Serial.println(data_S2); 
      Serial.print("status S2= ");Serial.println(status_S2); 
      Serial.println("------------------------------||-------------------------------\n");                                                  
      #endif //DEBUG
      } 
    }
  }
}


//==========================================================================================================================================//
//==================================================|     Procedure error data        |=====================================================//                                         
//==========================================================================================================================================//
void errorData(){
  if((millis() - currentMillis_errorData)>=5000){
    currentMillis_errorData = millis();
    errorCheck_S1++;
    errorCheck_S2++;
  }
  if(errorCheck_S1 == 3){
    status_S1 = 1;
    errorCheck_S1 = 0;
    Serial.print("status S1= ");Serial.println(status_S1); 
  }
  if(errorCheck_S2 == 3){
    status_S2 = 1;
    errorCheck_S2 = 0;
    Serial.print("status S2= ");Serial.println(status_S2); 
  }
}

//==========================================================================================================================================//
//=========================================================|   Setup Program    |===========================================================//                                         
//==========================================================================================================================================//
void setup(){
    Serial.begin(115200);
    Serial3.begin(115200);

    // reserve 200 bytes for the incomingData:
    incomingData.reserve(200);
}


//==========================================================================================================================================//
//===========================================================|   Main Loop    |=============================================================//                                         
//==========================================================================================================================================//
void loop(){
    // Serial.print(currentMillis);Serial.print(" | ");Serial.println(previousMillis);
    sendCommand();
    showData();
    // errorData();
}



//==========================================================================================================================================//
//==========================================================|   Serial ISR    |=============================================================//                                         
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
    if(inChar == 'A'){
      prefix_A = true;
      errorCheck_S1 = 0;
    } else {
      if(inChar == 'B'){
      prefix_B = true;
      errorCheck_S2 = 0;
      } else {
        if(inChar == '\n'){ 
          stringComplete = true;
        }
      }
    }
  }
}