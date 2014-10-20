// @ Harris Christiansen (HarrisChristiansen.com)
// 2014-09-27

#include <GPRSInterface.h>
#include <SoftwareSerial.h>
#include "arduino.h"

#define PIN_TX 7
#define PIN_RX 8

char get_cmd[] = "Get";
char buffer[512];

GPRSInterface gprs(PIN_TX,PIN_RX,19200,"phone",NULL,NULL);

void setup() {
  Serial.begin(19200);
  
  delay(1000);
  Serial.println("Starting");
  
  gprs.init();
  
  Serial.println("Init Complete");
  
  while(!gprs.connect()) {
    Serial.println("GPRS Connection Error");
    delay(2000);
  }
  
  Serial.print("Connected. IP Address: ");
  Serial.println(gprs.getIPAddress());
}

void loop() {
  TCPSocketConnection sock;
  if(!sock.connect("blue.harrischristiansen.com", 80)) {
    Serial.println("Web Connection Error");
  }
  
  Serial.println("Socket Connected");
  
  sock.send_all(get_cmd, sizeof(get_cmd)-1);
  
  Serial.println("Request Sent");
  
  while (true) {
    int ret = sock.receive(buffer, sizeof(buffer)-1);
    if (ret <= 0)
      break;
    buffer[ret] = '\0';
    Serial.print("Recv: ");
    Serial.print(ret);
    Serial.print(" bytes: ");
    Serial.println(buffer);
  }
  sock.close();
  Serial.println("Completed");
}
