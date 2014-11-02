// Harris Christiansen (HarrisChristiansen.com)
// 2014-10-31
// For YHack - Ouija Board

int motorX = 9;
int motorY = 10;

int x = 0, y = 0;

char runningQue[40];
char inChar=-1;
byte index = 0;

int motorSpeed = 50;

int coordinatesTable[40][3] = {
                200,50,'a', 250,200,'b', 300,200,'c', 350,200,'d', 400,200,'e', 450,200,'f', 500,200,'g', 550,200,'h',
  150,400,'i', 190,400,'j', 225,400,'k', 275,400,'l', 325,400,'m', 375,400,'n', 425,400,'o', 475,400,'p', 525,400,'q', 575,400,'r',
                200,600,'s', 250,600,'t', 300,600,'u', 350,600,'v', 400,600,'w', 450,600,'x', 500,600,'y', 550,600,'z',
  150,800,'0', 190,800,'1', 225,800,'2', 275,800,'3', 325,800,'4', 375,800,'5', 425,800,'6', 475,800,'7', 525,800,'8', 575,800,'9',
  150,50,'+',//Yo
  350,50,'&',//End of Sent
  0,0,'!',//Unused
  0,0,'.'//SPACE
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
    delay(300);
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
  if(nextPos == 39) { delay(1000); return; }
  
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
  analogWrite(motorX, (x<coordinatesTable[nextPos][0]) ? 5600 : 5528);
  analogWrite(motorY, (y<coordinatesTable[nextPos][1]) ? 5528 : 5600);
  
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
