#ifndef Leg_h
#define Leg_h

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

class Leg {
  public:
    Leg(Adafruit_PWMServoDriver pwmServoDriver, int servoPin, int limitSwitchPin, unsigned long timeToMoveForwardMs);
    void periodicUpdate();
    boolean isAtLimit();
    boolean isMovingForward();
  private:
    Adafruit_PWMServoDriver pwm;
    int servo;
    int limitSwitch;
    unsigned long fullExtensionTime;
    unsigned long startOfMovementTime;
    boolean movingForward;
    boolean previousLimitSwitch;
};

#endif
