// @ Harris Christiansen (HarrisChristiansen.com)
// 2014-09-26

int pwmIN = 3;
int pwmOut = 9;
int resultIN = 10;

int currentSpeed = 1500;

void setup() {
  pinMode(pwmIN, INPUT);
  pinMode(resultIN, INPUT);
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  //analogWrite(pwmOut, pulseIn(pwmIN, HIGH)/7);
  Serial.println(pulseIn(resultIN, HIGH));
  /*if(Serial.available() > 0) {
    currentSpeed = Serial.parseInt();
    Serial.println(currentSpeed);
  }
  analogWrite(pwmOut, currentSpeed);*/
  delay(100);
}
