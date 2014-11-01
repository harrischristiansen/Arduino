// Harris Christiansen (HarrisChristiansen.com)
// 2014-10-31
// For YHack - Ouija Board

int motorX = 9;
int motorY = 10;

int x = 0, y = 0;

char runningQue[40];
char inChar=-1;
byte index = 0;

int motorSpeed = 1000; // 100 Units/Sec

int coordinatesTable[39][3] = {
  100,100,'a',
  200,200,'b',
  300,300,'c',
  400,400,'d',
  500,500,'e',
  600,600,'f',
  700,700,'g',
  800,800,'h',
  900,900,'i',
  1000,1000,'j',
  1100,1100,'k',
  1200,1200,'l',
  1300,1300,'m',
  1400,1400,'n',
  1500,1500,'o',
  1600,1600,'p',
  1700,1700,'q',
  1800,1800,'r',
  1900,1900,'s',
  2000,2000,'t',
  2100,2100,'u',
  2200,2200,'v',
  2300,2300,'w',
  2400,2400,'x',
  2500,2500,'y',
  2600,2600,'z',
  2700,2700,'0',
  2800,2800,'1',
  2900,2900,'2',
  3000,3000,'3',
  3100,3100,'4',
  3200,3200,'5',
  3300,3300,'6',
  3400,3400,'8',
  3500,3500,'9',
  3600,3600,'+',//YES
  3700,3700,'-',//NO
  3800,3800,'!',//GOODBYE
  3900,3900,'.'//SPACE
};

void setup() {
  Serial.begin(57600);
  runningQue[0] = '\0';
}

void loop() {
  delay(20);
  readSerial();
  while(runningQue[0] != '\0') {
    printArray();
    goTo();
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
  if(nextPos>40 || nextPos<0) { return; }
  
  Serial.print("At: ");
  Serial.print(x);
  Serial.print(" Moving To: ");
  Serial.print(coordinatesTable[nextPos][0]);
  
  // Determine Time In Each Direction
  int xMoveTime = 1000*(abs(x-coordinatesTable[nextPos][0])/(float)motorSpeed);
  int yMoveTime = 1000*(abs(y-coordinatesTable[nextPos][1])/(float)motorSpeed);
  Serial.print(" Moving: ");
  Serial.println(xMoveTime);
  
  // Set Movement In Direction
  analogWrite(motorX, (x<coordinatesTable[nextPos][0]) ? 5500 : 5600);
  analogWrite(motorY, (y<coordinatesTable[nextPos][1]) ? 5500 : 5600);
  
  // Stop Movement In First Direction
  delay((xMoveTime<yMoveTime)?xMoveTime:yMoveTime);
  
  if(xMoveTime<yMoveTime) { // Stop X Motor
    analogWrite(motorX, 1535);
    delay(yMoveTime-xMoveTime);
  } else { // Stop Y Motor
    analogWrite(motorY, 1535);
    delay(xMoveTime-yMoveTime);
  }
  
  // Stop Movement In Second Direction
  analogWrite(motorX, 1535);
  analogWrite(motorY, 1535);
  
  // Update X and Y Position
  x = coordinatesTable[nextPos][0];
  y = coordinatesTable[nextPos][1];
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
