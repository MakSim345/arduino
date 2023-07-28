/*
 An example of uln2003 motor
 Project: Roller Blinds
 MAY-2021
 http://robotosha.ru/arduino/stepper-motor-28byj-uln2003-arduino.html
 */

#include "AccelStepper.h"
// #include <AccelStepper.h>

#define HALFSTEP 8

// Set pins for motor:
#define motorPin1   8  // IN1 on the 1-st driver ULN2003
#define motorPin2   9  // IN2 on the 1-st driver ULN2003
#define motorPin3   10 // IN3 on the 1-st driver ULN2003
#define motorPin4   11 // IN4 on the 1-st driver ULN2003

// Pins sequense: IN1-IN3-IN2-IN4
// AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

const int DISTANCE = 1000; // 20000

void setup()
{
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(500.0);
  stepper1.setSpeed(200);
  stepper1.moveTo(DISTANCE); // set distanceToGo

  Serial.begin(9600);
}

void loop()
{
  Serial.println(stepper1.distanceToGo());
  // change direction when stepper got the limit:
  if(stepper1.distanceToGo() == 0)
  {
    stepper1.moveTo(-stepper1.currentPosition());
  }

  stepper1.run();
}
