#include <SoftwareSerial.h>
#include <String.h> 
SoftwareSerial mySerial(7,8);
 
void setup() {
  mySerial.begin(19200);
  Serial.begin(19200);
  delay(500);
}
 
void loop()
{
  if (Serial.available()) {
    switch(Serial.read()) {
     case 't':
       SendTextMessage();
       break;
     case 'd':
       DialVoiceCall();
       break;
    }
  }
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
 
///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{
  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  delay(100);
  mySerial.println("AT + CMGS = \"+18017066385\"");//send sms message, be careful need to add a country code before the cellphone number
  delay(100);
  mySerial.println("From The Quadcopter");//the content of the message
  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  mySerial.println();
}
 
///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall()
{
  mySerial.println("ATD + +18017066385;");//dial the number
  delay(100);
  mySerial.println();
}
 
void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}
