// @ Harris Christiansen (HarrisChristiansen.com)
// 2014-09-26

int pwmIN = 3;
int pwmOut = 9;
int resultIN = 10;

void setup() {
  pinMode(pwmIN, INPUT);
  pinMode(resultIN, INPUT);
  Serial.begin(57600);
}

void loop() {
  //analogWrite(pwmOut, pulseIn(pwmIN, HIGH)/7);
  //Serial.println(pulseIn(resultIN, HIGH));
  Serial.println(analogRead(3));
  delay(100);
}
