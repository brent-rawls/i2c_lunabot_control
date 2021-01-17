/* Lunabot Master Arduino
 * Wheel test code
 * 
 * written: March 2020
 * 
 * Tests drive motor and servo functions for test rig
 * 
 * Brent wrote this code in haste; forgive these sins
 *
 */


#include <Wire.h>

//
#define   FRONT_WHEEL_CONTROLLER  (10)
#define   REAR_WHEEL_CONTROLLER   (11)
//drive modes - combo of ENx1, ENx2
#define   FWD     (1)
#define   REV     (2)
#define   BRAKE   (3)
#define   COAST   (0)



void setup(){
  Wire.begin(); // join i2c bus (address optional for master)
}

byte motorValue = 128;
byte servoAngle = 128;  //use 128 for neutral angle, "0 degrees", +/- 90 or so

//derived values for transmit only
byte driverMode = COAST;
byte driveSpeed = 0;

//for generating test values only
int generateServoDirection = 1;
int generateMotorValue = 1;

void loop(){

  motorValue = analogRead(A1);
  servoAngle = analogRead(A2);
  
  if(motorValue > 138){
     driverMode = FWD;
     driveSpeed = map(motorValue, 138, 255, 0, 255);
  }
  else if(motorValue < 118){
     driverMode = REV;
     driveSpeed = map(motorValue, 118, 0, 0, 255);
  }
  else{
     driverMode = BRAKE;
     driveSpeed = 0;
  }

  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  Wire.beginTransmission(FRONT_WHEEL_CONTROLLER); 
  Wire.write(driverMode);
  Wire.write(driveSpeed);
  Wire.write(servoAngle);
  Wire.endTransmission();    


  Wire.beginTransmission(REAR_WHEEL_CONTROLLER); 
  Wire.write(driverMode);
  Wire.write(driveSpeed);
  Wire.write(servoAngle);
  Wire.endTransmission();   

/*
  //this stuff just generates angles and speeds for testing
  servoAngle += generateServoDirection;
  if(servoAngle > (128 + 90) || servoAngle < (128 - 90)) generateServoDirection *= -1; //don't exceed 90deg
  motorValue += generateMotorValue;
  if(motorValue >=255 || motorValue <= 0) generateMotorValue *= -1;
  */
  
  delay(100);
}
