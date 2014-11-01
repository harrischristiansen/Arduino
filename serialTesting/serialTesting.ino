// @Harris Christiansen (HarrisChristiansen.com)
// 2014-10-03
// Serial Testing At HackMizzou

void setup() {
  Serial.begin(57600);
  Serial.println("Started");
}

char inChar=-1;
bool justIn = false;

void loop() {
  delay(20);
  while(Serial.available() > 0) {
   inChar = Serial.read();
   Serial.print(inChar);
   justIn = true;
 }
 if(justIn) {
   Serial.println("");
   justIn=false;
 }
}
