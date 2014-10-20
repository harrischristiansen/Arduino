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

//////////////////////////////////////////

// Create Sensors
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

// Calculated Values
float frontLeft=0, frontRight=0, backLeft=0, backRight=0;
float roll=0, pitch=0;

void setup() {
  Serial.begin(115200);
  pinMode(toggleIn, INPUT);
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
  delay(50);
}

////////////// Flight Controls ///////////////


float holdSpeed = 0;
float rollCorrect = 0;
float pitchCorrect = 0;
void calculateHold() {
  holdSpeed=32;
  pitchCorrect = ((roll/3.5)+pitchCorrect)/2;
  rollCorrect = ((pitch/3.5)+rollCorrect)/2;
  
  frontLeft = ((holdSpeed + rollCorrect - pitchCorrect)+frontLeft)/2;
  frontRight = ((holdSpeed - rollCorrect - pitchCorrect)+frontRight)/2;
  backLeft = ((holdSpeed + rollCorrect + pitchCorrect)+backLeft)/2;
  backRight = ((holdSpeed - rollCorrect + pitchCorrect)+backRight)/2;
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

void updateMotors() {
  analogWrite(frontLeftPWM, ((int)frontLeft)+135);
  analogWrite(frontRightPWM, ((int)frontRight)+135);
  analogWrite(backLeftPWM, ((int)backLeft)+135);
  analogWrite(backRightPWM, ((int)backRight)+135);
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
  
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation)) {
    roll = orientation.roll; // Get Roll
    pitch = orientation.pitch; // Get Pitch
  }
}
