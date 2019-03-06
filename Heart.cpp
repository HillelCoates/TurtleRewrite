#include "Arduino.h"
#include "Heart.h"

Heart::Heart(int heartPin) {
  pin = heartPin;
}

void Heart::periodicUpdate() {
  switch (currentMood) {
    case CALM:
      break;
    case HUNT:
      break;
    case SCARED:
      break;
  }
}

void Heart::setMode(Mood mood) {
  currentMood = mood;
}
