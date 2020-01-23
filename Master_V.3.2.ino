/*
Author  : Namira Fathani
Date    : 11/12/2019
*/


//==========================================================================================================================================//
//===================================================|   Initialization Program    |========================================================//                                         
//==========================================================================================================================================//
#include "RTClib.h"
#define BUTTON_1 50
#define RS485 48
#include <Wire.h>
#include <LiquidCrystal.h>


int readButtonCurrent_1;
int readButtonLast_1;

const int buttonPin = 50; //button yang digunakan untuk mengetahui nilai dari sensor 1
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7; //inisialisasi pin LCD
RTC_DS1307 RTC;  
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

bool pushbutton_1 = false;

String data;                                                        // pembacaan data sensor tanpa tahu header
String dataSensor1, dataSensor2;                                    // compared the value of reading data sensor in (dataSensor1 for sensor 1, dataSensor2 for sensor 2)
String data1, data2;
float keliling = 3.14*15;                                           // circumference of pipe
unsigned long previousMillis = 0;                                   // previous millis for sensor 
unsigned long previousMillis1 = 0;
bool M1, M2 = false;
//==========================================================================================================================================//
//=================================================|   Procedure Read Serial 3    |=========================================================//                                         
//==========================================================================================================================================//
void showData(){
  if(Serial3.available()){
    String data = Serial3.readString();                             // read data from serial 3. variable data is String
    Serial.println(data);                                           // for debugging. the aim is to know what the header is it
    data.remove(data.length()-1, data.length() - 0);
      if(data.indexOf('R')){                                        // if there is character/string 'R' in data. That is a dataSensor1
      dataSensor1 = data;                                           // value of dataSensor header S  
//      dataSensorA += data;                                        // value for sending data to Serial2                   
//      Serial2.print('S');
//      Serial2.print(dataSensorA);
      
//      Serial2.print('\n');
//      Serial.print('R');
//      Serial.println(dataSensorA);
//      Serial.print('\n');
      }else
      {
      dataSensor2 = data;                           //value of dataSensor header R
//      dataSensorB += data;
//      Serial2.print('R');
//      Serial2.print(dataSensorB);
//      Serial2.print('\n');
//      Serial.print('R');
//      Serial.println(dataSensorB);
//      Serial.print('\n');
     }
    data.remove(0,1);                           //process the string to get the value of sensor
    Serial.println(data);
    Serial.println(dataSensor1);
    Serial.println(dataSensor2);
//    LcdPrintTime(); 
  }
}


//==========================================================================================================================================//
//====================================================|   Procedure pushbutton    |=========================================================//                                         
//==========================================================================================================================================//
void lcd1show(){                  //for pushbutton 1
  lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ID : M1");
        LcdPrintTime();
        lcd.setCursor(0,2);
        lcd.print("Count : ");   
        lcd.print(dataSensor1.toInt());               
        lcd.setCursor(0,3);
        lcd.print("Panjang : ");
        lcd.print(dataSensor1.toInt()*keliling); 
}

void lcd2show(){                  //for pushbutton 1
  lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ID : M2");
        LcdPrintTime();
        lcd.setCursor(0,2);
        lcd.print("Count : ");   
        lcd.print(dataSensor2.toInt());               
        lcd.setCursor(0,3);
        lcd.print("Panjang : ");
        lcd.print(dataSensor2.toInt()*keliling); 
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
//================================================|    Procedure lcd Get RealTimeClock   |======================================================//                                         
//==========================================================================================================================================//
void LcdPrintTime(){
  DateTime now = RTC.now();

  lcd.setCursor(0,1);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  lcd.print(" WIB");
  delay(1000);
}
//==========================================================================================================================================//
//================================================|     Procedure Push Button        |======================================================//                                         
//==========================================================================================================================================//
    void buttonread(){
    readButtonCurrent_1 =
 digitalRead(BUTTON_1);
    if(readButtonCurrent_1 == HIGH){
        readButtonLast_1 = readButtonCurrent_1;  
    }
    if(readButtonCurrent_1 != readButtonLast_1){
        pushbutton_1 = true;
        readButtonLast_1 = readButtonCurrent_1;
    }

    }
//==========================================================================================================================================//
//=======================================================|   Setup Program    |=============================================================//                                         
//==========================================================================================================================================//
void setup(){
  Serial.begin(9600); //Kecepatan baudrate antara pc dan mega
    Serial2.begin(57600);
    Serial3.begin(9600);
    Wire.begin();
    RTC.begin();
    lcd.begin(20, 4);

    pinMode(BUTTON_1, INPUT);
    pinMode(RS485, OUTPUT);

}


//==========================================================================================================================================//
//====================================================|   Main Loop Program    |============================================================//                                         
//==========================================================================================================================================//
void loop(){
  EnableReceiver();
  unsigned long currentMillis = millis();           //times running
  
      if((currentMillis - previousMillis) == 6000){             //if the time has passed 1 second the function running
    EnableTransmitter();
    Serial3.print("SENSOR_1");
    Serial.println("command sending slave_1 !!!");
    delay(1000);
    EnableReceiver();                                                  
      }
    if((currentMillis - previousMillis) == 12000){               //if the time has passed 2 second the function running
          previousMillis = millis();                            //save the millis to previous millis (tidak ditaruh di akhir rangkaian if karena apabila di akhir waktu millis yang berjalan akan melebihi 2 detik)
          EnableTransmitter();
          Serial3.print("SENSOR_2");
          Serial.println("command sending slave_2 !!!");
          delay(1000);
          EnableReceiver();                                          
    }

    showData();

      
    // function read pushButton_1
     buttonread();
       


}
