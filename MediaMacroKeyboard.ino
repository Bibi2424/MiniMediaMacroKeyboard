/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  Improved Keyboard example

  Shows how to use the new Keyboard API.

  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Keyboard-API#improved-keyboard
*/

#include "HID-Project.h"
#include <Encoder.h>

const int pinLed = 9;
const int pinButton = 14;
const int pinButton2 = 15;
const int pinButton3 = 16;

Encoder myEnc(2, 3);

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinButton2, INPUT_PULLUP);
  pinMode(pinButton3, INPUT_PULLUP);

  // Sends a clean report to the host. This is important on any Arduino type.
  // Need booth Keyboard and Consumer since they are not capable of sending eachother keycode
  Keyboard.begin();
  Consumer.begin();
}


void debounce(int wait) {
    // Simple debounce
    delay(wait);
    digitalWrite(pinLed, LOW);
}

long oldPosition  = -999;

void loop() {
  if(!digitalRead(pinButton)) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_VOLUME_MUTE);
    debounce(300);
  }
  if(!digitalRead(pinButton2)) {
    digitalWrite(pinLed, HIGH);
    Consumer.write(MEDIA_NEXT);
    debounce(300);
  }
  if(!digitalRead(pinButton3)) {
    digitalWrite(pinLed, HIGH);
    Keyboard.write(KEY_A);
    debounce(300);
  }

  long newPosition = myEnc.read() / 4;
  if (newPosition != oldPosition) {
    long diff = newPosition - oldPosition;
    if(diff > 0) {
      Consumer.write(MEDIA_VOLUME_UP);
    }
    else {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    
    oldPosition = newPosition;
  }
}
