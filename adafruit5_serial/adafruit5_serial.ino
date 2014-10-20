int led=13;

void setup() {
  pinMode(led, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Started");
}

boolean ledON=false;
void loop() {
  if (Serial.available()) {
    char ch = Serial.read();
    
    ledON=!ledON;
    if(ledON) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
  }
}
