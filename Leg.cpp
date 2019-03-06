#include "Arduino.h"
#include "Leg.h"
#include "Adafruit_PWMServoDriver.h"

#define SERVOMIN 150
#define SERVOMAX 150

Leg::Leg(Adafruit_PWMServoDriver pwmServoDriver, int servoPin, int limitSwitchPin, unsigned long timeToMoveForwardMs) {
  pwm = pwmServoDriver;
  servo = servoPin;
  limitSwitch = limitSwitchPin;
  pinMode(limitSwitchPin, INPUT);
  fullExtensionTime = timeToMoveForwardMs;
}

void Leg::periodicUpdate() {
  boolean currentLimitSwitch = this->isAtLimit();
  unsigned long currentTime = millis();
  if (currentLimitSwitch && !previousLimitSwitch) {
    movingForward = true;
    startOfMovementTime = currentTime;
  }

  if (currentTime - startOfMovementTime > fullExtensionTime && movingForward) {
    movingForward = false;
  }

  pwm.setPWM(servo, 0, movingForward ? SERVOMAX : SERVOMIN);
  previousLimitSwitch = currentLimitSwitch;
}

boolean Leg::isAtLimit() {
  digitalRead(limitSwitch);
}

boolean Leg::isMovingForward() {
  return movingForward;
}
