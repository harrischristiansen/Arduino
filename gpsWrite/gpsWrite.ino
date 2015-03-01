#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <FileIO.h>

SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

void setup() {
  Bridge.begin();
  FileSystem.begin();

  GPS.begin(9600);
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);
}

uint32_t timer = millis();
void loop() {
  char c = GPS.read();
  
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  if (timer > millis())  timer = millis();

  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer
    
    File script = FileSystem.open("/tmp/gpsLoc.txt", FILE_WRITE);
    
    
    script.print("\nTime: ");
    script.print(GPS.hour, DEC); script.print(':');
    script.print(GPS.minute, DEC); script.print(':');
    script.print(GPS.seconds, DEC); script.print('.');
    script.println(GPS.milliseconds);
    script.print("Date: ");
    script.print(GPS.day, DEC); script.print('/');
    script.print(GPS.month, DEC); script.print("/20");
    script.println(GPS.year, DEC);
    script.print("Fix: "); script.print((int)GPS.fix);
    script.print(" quality: "); script.println((int)GPS.fixquality);
   
    if (GPS.fix) {
      script.print("Location: ");
      script.print(GPS.latitude, 4); script.print(GPS.lat);
      script.print(", "); 
      script.print(GPS.longitude, 4); script.println(GPS.lon);
      
      
      script.print("Speed (knots): "); script.println(GPS.speed);
      script.print("Angle: "); script.println(GPS.angle);
      script.print("Altitude: "); script.println(GPS.altitude);
      script.print("Satellites: "); script.println((int)GPS.satellites);
      
    }
    
    script.close();
  }
}
