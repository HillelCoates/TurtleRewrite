#include "Arduino.h"
#include "Heart.h"

Heart::Heart(int heartPin) {
  pin = heartPin;
}

void Heart::periodicUpdate() {
  /*fadeValue += fadeAmount
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
  }

  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }*/
}

void Heart::setMode(Mood mood) {
  currentMood = mood;
  switch (mood) {
    case CALM:
      fadeAmount = 2;
      break;
    case HUNT:
      fadeAmount = 3;
      break;
    case SCARED:
      fadeAmount = 5;
      break;
  }
}
