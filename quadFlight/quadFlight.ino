// Harris Christiansen (HarrisChristiansen.com)
// 2014-10-13
// iDrone.com

// Includes
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_10DOF.h>

//////////////// Settings ////////////////

int frontLeftPWM=11, frontRightPWM=3, backLeftPWM=10, backRightPWM=9; // PWM Ports
int toggleIn = 5;
float desiredAltitude=1;

//////////////////////////////////////////

// Create Sensors
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

// Calculated Values
float startAltitude = 0;
float frontLeft=0, frontRight=0, backLeft=0, backRight=0;
float roll=0, pitch=0, heading=0, alt=0, temp=0;

void setup() {
  Serial.begin(115200);
  pinMode(toggleIn, INPUT);
  pinMode(13, OUTPUT);
  initSensors(); // Initialize Sensors
}

void loop() {
  if(pulseIn(toggleIn, HIGH)<1200) { //// 0 Signal
    zeroRoutine();
  } else { //// Flight Routine
    updateSensors();
    calculateHold();
    updateMotors();
  }
}

////////////// Zero Routine //////////////
void zeroRoutine() {
  analogWrite(frontLeftPWM, 135);
  analogWrite(frontRightPWM, 135);
  analogWrite(backLeftPWM, 135);
  analogWrite(backRightPWM, 135);
  
  // Reset Start Altitude
  startAltitude=0;
  for(int i=0;i<10;i++) {
    updateSensors();
    startAltitude += alt;
    delay(10);
  }
  
  startAltitude = startAltitude/10;
  Serial.print("Start Altitude: "); Serial.println(startAltitude);
}

////////////// Flight Controls ///////////////

float altDiff = 0;
float holdSpeed = 0;
float rollCorrect = 0;
float pitchCorrect = 0;
float rotateCorrect = 0;
void calculateHold() {
  // **** Play With These Nums **** //
  altDiff = alt-(startAltitude+desiredAltitude);
  //holdSpeed = abs(altDiff)*10+50;
  holdSpeed=32;
  pitchCorrect = ((roll/3.5)+pitchCorrect)/2;
  rollCorrect = ((pitch/3.5)+rollCorrect)/2;
  //rotateCorrect = (heading > 180 ? heading-360 : heading)/20;
  rotateCorrect=0;
  
  frontLeft = ((holdSpeed + rollCorrect - pitchCorrect + rotateCorrect)+frontLeft)/2;
  frontRight = ((holdSpeed - rollCorrect - pitchCorrect - rotateCorrect)+frontRight)/2;
  backLeft = ((holdSpeed + rollCorrect + pitchCorrect - rotateCorrect)+backLeft)/2;
  backRight = ((holdSpeed - rollCorrect + pitchCorrect + rotateCorrect)+backRight)/2;
  frontLeft = restrictRange(frontLeft);
  frontRight = restrictRange(frontRight);
  backLeft = restrictRange(backLeft);
  backRight = restrictRange(backRight);
  
  Serial.print(frontLeft); Serial.print(" ");
  Serial.print(frontRight); Serial.print(" ");
  Serial.print(backLeft); Serial.print(" ");
  Serial.println(backRight);
  
}

float restrictRange(float value) {
  value = value < 0 ? 0 : value;
  return value > 100 ? 100 : value;
}

boolean ledOn = false;
void updateMotors() {
  analogWrite(frontLeftPWM, ((int)frontLeft)+135);
  analogWrite(frontRightPWM, ((int)frontRight)+135);
  analogWrite(backLeftPWM, ((int)backLeft)+135);
  analogWrite(backRightPWM, ((int)backRight)+135);
  if(ledOn) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
  ledOn = !ledOn;
}


//////// -------- Sensors ------- ////////
void initSensors() {
  if(!accel.begin()) {
    Serial.println(F("No LSM303 detected - accel"));
    while(1);
  }
  if(!mag.begin()) {
    Serial.println("No LSM303 detected - mag");
    while(1);
  }
  if(!bmp.begin()) {
    Serial.println("No BMP180 detected");
    while(1);
  }
}

void updateSensors() {
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  float rollCng=0, pitchCng=0;
  for(int i=0;i<10;i++) {
    accel.getEvent(&accel_event);
    if (dof.accelGetOrientation(&accel_event, &orientation)) {
      rollCng += orientation.roll; // Get Roll
      pitchCng += orientation.pitch; // Get Pitch
    }
  }
  roll += rollCng/10;
  pitch += pitchCng/10;
  roll = roll/2;
  pitch = pitch/2;
  
  /*
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
    heading = orientation.heading; // Get Heading
  }

  bmp.getEvent(&bmp_event);
  if (bmp_event.pressure) {
    bmp.getTemperature(&temp); // Get Temperature
    alt += bmp.pressureToAltitude(seaLevelPressure, bmp_event.pressure, temp); // Get Altitude
    alt = alt/2;
  }
  */
}
