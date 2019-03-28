#include "Adafruit_PWMServoDriver.h"
#include "Leg.h"
#include "Swerve.h"
#include "Heart.h"
#include "Head.h"
#include "Tail.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

unsigned long tracks[] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};

int track = 0;

const int lightPin = A0;
const int buttonPin = 12;

typedef enum {FORWARD, AVOIDANCE, HUNTING, SPEAKING} Mode;

Mode currentMode = FORWARD;

unsigned long modeStartTime;

int submodeStep;
int huntingStep;

bool preButtonPressed;

bool moveLegs;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

SoftwareSerial softwareSerial(10, 11);
DFRobotDFPlayerMini player;

Leg frontLeft(pwm, 0, 2, 300, false);
Leg frontRight(pwm, 1, 3, 300, true);
Leg backLeft(pwm, 2, 4, 300, false);
Leg backRight(pwm, 3, 5, 300, true);

Swerve swerve(pwm, 14, 15, 11);

Tail tail(pwm, 12, 13);

Head head(pwm, 10);

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);

  if (!player.begin(softwareSerial)) {
    Serial.println("DFPlayerMini failed to begin");
  }

  player.volume(30);

  randomSeed(analogRead(A5));

  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  swerve.setDrive(0, 0);

  while (!frontLeft.isAtLimit() || !frontRight.isAtLimit() || !backLeft.isAtLimit() || !backRight.isAtLimit()) {
    Serial.println("Zeroing legs");
    frontLeft.zero();
    frontRight.zero();
    backLeft.zero();
    backRight.zero();
    swerve.periodicUpdate();
    tail.periodicUpdate();
  } //TODO

  moveLegs = true;//TODO
}

void loop() {
  unsigned long currentTime = millis();
  double distance = getDistance();
  bool huntingTriggered = getLight() > 800;
  bool buttonPressed = getButton();

  switch (currentMode) {
    case FORWARD:
      if (buttonPressed && !preButtonPressed) {
        Serial.println("Boring shit Triggered");
        currentMode = SPEAKING;
        moveLegs = false;
      } else if (huntingTriggered) {
        Serial.println("Hunting Triggered");
        currentMode = HUNTING;
        moveLegs = false;
      } else if (distance < 20) {
        Serial.println("Avoidance Triggered");
        currentMode = AVOIDANCE;
        moveLegs = false;
      } else {
        moveLegs = true;//TODO
        swerve.setDrive(0, 100);
      }
      break;
    case SPEAKING:
      switch (submodeStep) {
        case 0:
          swerve.setDrive(0, 0);
          moveLegs = false;
          if (!frontLeft.isAtLimit() || !frontRight.isAtLimit() || !backLeft.isAtLimit() || !backRight.isAtLimit()) {
            frontLeft.zero();
            frontRight.zero();
            backLeft.zero();
            backRight.zero();
          } else {//TODO
            track = random(sizeof(tracks) / sizeof(unsigned long));
            Serial.print("Playing track ");
            Serial.print(track + 1);
            Serial.print(" of length ");
            Serial.print(tracks[track]);
            player.play(track + 1);
            modeStartTime = currentTime;
            submodeStep++;
          }
          break;
        case 1:
          if (currentTime - modeStartTime > tracks[track]) {
            submodeStep++;
          }
          break;
        case 2:
          submodeStep = 0;
          currentMode = FORWARD;
          break;
      }
      break;
    case AVOIDANCE:
      switch (submodeStep) {
        case 0:
          swerve.setDrive(0, 0);
          moveLegs = false;
          if (!frontLeft.isAtLimit() || !frontRight.isAtLimit() || !backLeft.isAtLimit() || !backRight.isAtLimit()) {
            frontLeft.zero();
            frontRight.zero();
            backLeft.zero();
            backRight.zero();
          } else {//TODO
            head.addOpens(4);
            submodeStep++;
          }
          break;
        case 1:
          if (head.getOpens() == -1) {
            submodeStep++;
            modeStartTime = currentTime;
            tail.addWags(4);
          }
          break;
        case 2:
          if (tail.getWags() == 0) {
            modeStartTime = currentTime;
            submodeStep++;
          }
          break;
        case 3:
          moveLegs = true;//TODO
          if (currentTime - modeStartTime > 2000) {
            modeStartTime = currentTime;
            submodeStep++;
          } else {
            swerve.setDrive(90, -100);
          }
          break;
        case 4:
          if (currentTime - modeStartTime > 2000) {
            submodeStep++;
            modeStartTime = currentTime;
          } else {
            swerve.setDrive(-90, 100);
          }
          break;
        case 5:
          submodeStep = 0;
          currentMode = FORWARD;
          break;
      }
      break;
    case HUNTING:
      if (!huntingTriggered) {
        currentMode = AVOIDANCE;
      } else {
        if (head.getOpens() == -1) {
          head.addOpens(2);
        }
        swerve.setDrive(0, 0);
        moveLegs = false;
      }
      break;
  }

  preButtonPressed = buttonPressed;
  allPeriodic();
}

double getDistance() {
  digitalWrite(7, LOW);
  delayMicroseconds(5);
  digitalWrite(7, HIGH);
  delayMicroseconds(10);
  digitalWrite(7, LOW);

  double distance = pulseIn(8, HIGH, 5000) / 29.1 / 2;
  return distance <= 0 ? 200 : distance;
}

int getLight() {
  return analogRead(lightPin);
}

bool getButton() {
  return !digitalRead(buttonPin);
}

void allPeriodic() {
  frontLeft.runLeg(moveLegs);
  frontRight.runLeg(moveLegs);
  backLeft.runLeg(moveLegs);
  backRight.runLeg(moveLegs);

  frontLeft.periodicUpdate();
  frontRight.periodicUpdate();
  backLeft.periodicUpdate();
  backRight.periodicUpdate();

  swerve.periodicUpdate();
  tail.periodicUpdate();
  head.periodicUpdate();
}
