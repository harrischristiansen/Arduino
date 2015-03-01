#include <FileIO.h>

void setup() {
  // Setup Bridge (needed every time we communicate with the Arduino Yún)
  Bridge.begin();

  // Setup File IO
  FileSystem.begin();
}

int myVar = 1;

void loop() {
  File script = FileSystem.open("/tmp/myTest.txt", FILE_WRITE);
  script.print(myVar);
  script.print("\n");
  script.close();
  
  myVar++;
  delay(5000);
}

