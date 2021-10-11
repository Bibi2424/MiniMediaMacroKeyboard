/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  Improved Keyboard example

  Shows how to use the new Keyboard API.

  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Keyboard-API#improved-keyboard
*/

#include "HID-Project.h"
#include <FastLED.h>
#include <Encoder.h>

const int pinPlay = 16;
const int pinNext = 10;
const int pinMute = 14;
const int pinMacro1 = 8;
const int pinMacro2 = 15;
const int pinMacro3 = 9;
const int pinModifier = 7;

Encoder myEnc(3, 2);

#define LED_PIN     5
#define NUM_LEDS    12
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void setup() {
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.show();
    
  pinMode(pinPlay, INPUT_PULLUP);
  pinMode(pinNext, INPUT_PULLUP);
  pinMode(pinMute, INPUT_PULLUP);
  pinMode(pinMacro1, INPUT_PULLUP);
  pinMode(pinMacro2, INPUT_PULLUP);
  pinMode(pinMacro3, INPUT_PULLUP);
  pinMode(pinModifier, INPUT);

  // Sends a clean report to the host. This is important on any Arduino type.
  // Need booth Keyboard and Consumer since they are not capable of sending eachother keycode
  Keyboard.begin();
  Consumer.begin();
}


void debounce(int wait) {
    // Simple debounce
    delay(wait);
}

long oldPosition  = -999;

void loop() {
  if(!digitalRead(pinPlay)) {
    Consumer.write(MEDIA_PLAY_PAUSE);
    debounce(300);
  }
  if(!digitalRead(pinNext)) {
    Consumer.write(MEDIA_NEXT);
    debounce(300);
  }
  if(!digitalRead(pinMute)) {
    Consumer.write(MEDIA_VOLUME_MUTE);
    debounce(300);
  }
  if(!digitalRead(pinMacro1)) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('c');
    Keyboard.release(KEY_LEFT_CTRL);
    debounce(300);
  }
  if(!digitalRead(pinMacro2)) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('v');
    Keyboard.release(KEY_LEFT_CTRL);
    debounce(300);
  }
  if(!digitalRead(pinMacro3)) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('z');
    Keyboard.release(KEY_LEFT_CTRL);
    debounce(300);
  }
  if(digitalRead(pinModifier)) {
    debounce(100);
    for(uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;
    }
    FastLED.show();
    debounce(1000);
    for(uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
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
