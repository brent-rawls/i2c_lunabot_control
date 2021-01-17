/* Lunabot Wheel Arduino
 * 
 * written: March 2020
 * Brent wrote this code in haste; forgive these sins
 *
 */

#include <Wire.h>

#define   FRONT_WHEEL_CONTROLLER  (10)
#define   REAR_WHEEL_CONTROLLER   (11)

//motor pins - Enable[0:1] and PWM speed for motor driver A, B
#define   ENA0    (13)     //change these bullshit pin numbers
#define   ENA1    (12)
#define   PWMA    (11)
#define   ENB0    (10)     //change these bullshit pin numbers
#define   ENB1    (9)
#define   PWMB    (8)

//servo pins - PWM position for servo A, B
#define   POSA    (7)
#define   POSB    (6)

//drive modes - combo of ENx1, ENx2
#define   FWD     (1)
#define   REV     (2)
#define   BRAKE   (3)
#define   COAST   (0)

byte driverMode = COAST;
byte driveSpeed = 0;
byte servoAngle = 128;


//set up modes (for debugging):
const boolean suppressOutputSignals = false;
const boolean serialMonitorOutput = true;



void setup(){
  pinMode(ENA0, OUTPUT);
  pinMode(ENA1, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(ENB0, OUTPUT);
  pinMode(ENB1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(POSA, OUTPUT);
  pinMode(POSB, OUTPUT);

  //CHANGE THIS FOR FRONT/REAR ARDUINO
  Wire.begin(REAR_WHEEL_CONTROLLER);   

  Wire.onReceive(receiveEvent);
  Serial.begin(115200);          
}


void loop(){
  //everything done as events; loop() is empty
}


//run when data received through I2C
void receiveEvent(int howMany){
  
  while(Wire.available()){
    driverMode = Wire.read(); 
    driveSpeed = Wire.read();
    servoAngle = Wire.read();
  }

  if(!suppressOutputSignals){
    setOutputs();
  }

  if(serialMonitorOutput){
    displaySerialOutput();
  }
}


//sets motor mode, speed; servo angle
void setOutputs(){
  //set motor driver mode
  if(driverMode == COAST){
    digitalWrite(ENA0, LOW);
    digitalWrite(ENA1, LOW);
    digitalWrite(ENB0, LOW);
    digitalWrite(ENB1, LOW);
  }
  else if(driverMode == FWD){
    digitalWrite(ENA0, LOW);
    digitalWrite(ENA1, HIGH);
    digitalWrite(ENB0, LOW);
    digitalWrite(ENB1, HIGH);    
  }
  else if(driverMode == REV){
    digitalWrite(ENA0, HIGH);
    digitalWrite(ENA1, LOW);
    digitalWrite(ENB0, HIGH);
    digitalWrite(ENB1, LOW);        
  }
  else{ //brake
    digitalWrite(ENA0, HIGH);
    digitalWrite(ENA1, HIGH);
    digitalWrite(ENB0, HIGH);
    digitalWrite(ENB1, HIGH);    
    
  }
  
  //set motor speed
  analogWrite(PWMA, driveSpeed);
  analogWrite(PWMB, driveSpeed);

  //set angle
  analogWrite(POSA, servoAngle);
  analogWrite(POSB, servoAngle);
  
}

//prints motor/servo values to serial monitor for debugging purposes
void displaySerialOutput(){
  Serial.print("mode: ");
  if(driveSpeed == 0 || driverMode == BRAKE){
    Serial.print("BRAKE");  
  }
  else if(driverMode == FWD){
    Serial.print("FWD");  
  }
  else if(driverMode == REV){
    Serial.print("REV");  
  }
  else{
    Serial.print("COAST");
  }
  Serial.print("\t");
  
  Serial.print("speed: ");
  Serial.print(driveSpeed);
  Serial.print("\t");

  Serial.print("servo: ");
  Serial.print(servoAngle);
  Serial.print("\n\n");
}
