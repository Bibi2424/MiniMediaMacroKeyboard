#include <Arduino.h>
#include <YetAnotherPcInt.h>
#include <HID-Project.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "button_action.h"
#include "shortcuts.h"
#include "nvm.h"


#define DEBOUNCE_TIME      100

//! TEMP
#define NUM_LEDS 12
extern CRGB leds[];


const int pinPlay = 16;
const int pinNext = 10;
const int pinMute = 14;
const int pinMacro1 = 8;
const int pinMacro2 = 15;
const int pinMacro3 = 9;
const int pinModifier = 7;

static bool modifier_pressed = false;


static void pinPlayChanged(bool new_state);
static void pinNextChanged(bool new_state);
static void pinMuteChanged(bool new_state);
static void pinMacro1Changed(bool new_state);
static void pinMacro2Changed(bool new_state);
static void pinMacro3Changed(bool new_state);
static void pinModifierChanged(void);


extern void button_init(void) {
    pinMode(pinPlay, INPUT_PULLUP);
    PcInt::attachInterrupt(pinPlay, pinPlayChanged, CHANGE);
    pinMode(pinNext, INPUT_PULLUP);
    PcInt::attachInterrupt(pinNext, pinNextChanged, CHANGE);
    pinMode(pinMute, INPUT_PULLUP);
    PcInt::attachInterrupt(pinMute, pinMuteChanged, CHANGE);
    pinMode(pinMacro1, INPUT_PULLUP);
    PcInt::attachInterrupt(pinMacro1, pinMacro1Changed, CHANGE);
    pinMode(pinMacro2, INPUT_PULLUP);
    PcInt::attachInterrupt(pinMacro2, pinMacro2Changed, CHANGE);
    pinMode(pinMacro3, INPUT_PULLUP);
    PcInt::attachInterrupt(pinMacro3, pinMacro3Changed, CHANGE);

    pinMode(pinModifier, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinModifier), pinModifierChanged, CHANGE);
}


extern bool is_modifier_pressed(void) {
	return modifier_pressed;
}


static void pinPlayChanged(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            Consumer.write(MEDIA_PLAY_PAUSE);
            Serial.println("Play");
        }
    }
    last_press = millis();
}


static void pinNextChanged(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            Consumer.write(MEDIA_NEXT);
            Serial.println("Next");
        }
    }
    last_press = millis();
}


static void pinMuteChanged(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            Consumer.write(MEDIA_VOLUME_MUTE);
            Serial.println("Mute");
        }
    }
    last_press = millis();
}


static void pinMacro1Changed(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            run_shortcut(COPY, nvm_config.os);
            Serial.println("Copy");
        }
    }
    else {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            nvm_config.os = !nvm_config.os;
            nvm_write(nvm_config);
            Serial.println("OS");
        }
    }
    last_press = millis();
}


static void pinMacro2Changed(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            run_shortcut(PASTE, nvm_config.os);
            Serial.println("Paste");
        }
    }
    last_press = millis();
}


static void pinMacro3Changed(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            run_shortcut(ZOOM_MUTE, nvm_config.os);
            Serial.println("ZOOM_MUTE");
        }
    }
    last_press = millis();
}


static void pinModifierChanged() {
    static uint32_t last_press = 0;

    if(digitalRead(pinModifier) && millis() - last_press > DEBOUNCE_TIME) {
        modifier_pressed = true;
        for(uint8_t i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB::Blue;
        }
        FastLED.setBrightness(  128 );
        FastLED.show();
    }
    else {
        modifier_pressed = false;
        FastLED.setBrightness(50);
    }

    last_press = millis();
}
