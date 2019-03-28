#include "Arduino.h"
#include "Leg.h"
#include "Adafruit_PWMServoDriver.h"

#define SERVOMIN 150
#define SERVOMAX 650

Leg::Leg(Adafruit_PWMServoDriver pwmServoDriver, int servoPin, int limitSwitchPin, unsigned long timeToMoveForwardMs, bool reverse) {
  pwm = pwmServoDriver;
  servo = servoPin;
  flip = reverse;
  limitSwitch = limitSwitchPin;
  fullExtensionTime = timeToMoveForwardMs;
  movingForward = true;
  pinMode(limitSwitch, INPUT_PULLUP);
}

void Leg::periodicUpdate() {
  if (enabled) {
    bool currentLimitSwitch = isAtLimit();
    unsigned long currentTime = millis();
    if (currentLimitSwitch && !previousLimitSwitch) {
      movingForward = true;
      startOfMovementTime = currentTime;
    }

    if (currentTime - startOfMovementTime > fullExtensionTime && movingForward) {
      movingForward = false;
    }
    pwm.setPWM(servo, 0, movingForward ? (flip ? SERVOMAX : SERVOMIN) : (flip ? SERVOMIN : SERVOMAX));
    previousLimitSwitch = currentLimitSwitch;
  }
}

void Leg::zero() {
  movingForward = false;
  pwm.setPWM(servo, 0, isAtLimit() ? 0 : (flip ? SERVOMIN : SERVOMAX));
}

void Leg::runLeg(bool enable) {
  movingForward = true;
  enabled = enable;
}

bool Leg::isAtLimit() {
  return !digitalRead(limitSwitch);
}

bool Leg::isMovingForward() {
  return movingForward;
}
