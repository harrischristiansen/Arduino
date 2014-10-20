int led=13;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  for(int i=0;i<4;i++) {
    digitalWrite(led, HIGH);
    delay(370);
    digitalWrite(led, LOW);
    delay(350);
  }
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(600);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(1000);
}
