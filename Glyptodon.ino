#include "Adafruit_PWMServoDriver.h"
#include "Leg.h"
#include "Heart.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Leg frontLeft;
Leg frontRight;
Leg backLeft;
Leg backRight;

Heart heart = Heart(8);

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);

  frontLeft = Leg(pwm, 0, 1, 1000);
  frontRight = Leg(pwm, 2, 3, 1000);
  backLeft = Leg(pwm, 4, 5, 1000);
  backRight = Leg(pwm, 6, 7, 1000);
}

void loop() {
  frontLeft.periodicUpdate();
  frontRight.periodicUpdate();
  backLeft.periodicUpdate();
  backRight.periodicUpdate();
  heart.periodicUpdate();
}
