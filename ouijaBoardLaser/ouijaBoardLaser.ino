// Harris Christiansen (HarrisChristiansen.com)
// 2014-10-31
// For YHack - Ouija Board

#include <Servo.h>

Servo servoX, servoY;

int motorX = 4;
int motorY = 8;

char runningQue[40];
char inChar=-1;
byte index = 0;

int coordinatesTable[40][3] = {
              85,162,'a', 78,162,'b', 71,162,'c', 65,162,'d', 58,162,'e', 52,162,'f', 46,162,'g', 40,162,'h',
  89,168,'i', 85,168,'j', 78,168,'k', 71,168,'l', 65,168,'m', 58,168,'n', 52,168,'o', 46,168,'p', 40,168,'q', 35,168,'r',
              85,174,'s', 78,174,'t', 71,174,'u', 65,174,'v', 58,174,'w', 52,174,'x', 46,174,'y', 40,174,'z',
  89,180,'0', 85,180,'1', 78,180,'2', 71,180,'3', 65,180,'4', 58,180,'5', 52,180,'6', 46,180,'7', 40,180,'8', 35,180,'9',
  90,155,'+',//Yo
  34,155,'&',//End of Sent
  0,0,'!',//Do Corners
  0,0,'.'//SPACE
};

void setup() {
  Serial.begin(57600);
  runningQue[0] = '\0';
  servoX.attach(motorX);
  servoY.attach(motorY);
}

void loop() {
  delay(20);
  readSerial();
  while(runningQue[0] != '\0') {
    printArray();
    goTo();
    delay(600);
  }
}

void readSerial() {
  while(Serial.available() > 0) {
    if(index < 39) {
      inChar = Serial.read();
      runningQue[index] = inChar;
      index++;
      runningQue[index] = '\0';
    }
  }
  index = 0;
}

void printArray() {
  Serial.println(runningQue);
}

void goTo() {
  int nextPos = getNextPos();
  if(nextPos>39 || nextPos<0) { return; }
  if(nextPos == 39) { return; }
  if(nextPos == 38) { // Show Corners
    servoX.write(85);
    servoY.write(162);
    delay(1000);
    servoX.write(40);
    servoY.write(162);
    delay(1000);
    servoX.write(35);
    servoY.write(180);
    delay(1000);
    servoX.write(89);
    servoY.write(180);
    delay(1000);
    return;
  }
  
  Serial.print("Moving To: ");
  Serial.print(coordinatesTable[nextPos][0]);
  Serial.print(" ");
  Serial.println(coordinatesTable[nextPos][1]);
  
  // Set Angles
  servoX.write(coordinatesTable[nextPos][0]);
  servoY.write(coordinatesTable[nextPos][1]);
  delay(100);
  servoX.write(coordinatesTable[nextPos][0]);
  servoY.write(coordinatesTable[nextPos][1]);
}

char getNextPos() {
  char returnChar = runningQue[0];
  for(int i = 0; i < 39; i++) {
    runningQue[i] = runningQue[i+1];
  }
  
  for(int i = 0; i <= 39; i++) {
    if(returnChar == (char)coordinatesTable[i][2]) {
      return i;
    }
  }
  
  return -1; // Char Not Found
}
