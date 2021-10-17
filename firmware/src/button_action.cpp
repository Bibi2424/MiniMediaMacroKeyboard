#include <Arduino.h>
#include <YetAnotherPcInt.h>
#include <HID-Project.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "effect_engine.h"
#include "button_action.h"
#include "shortcuts.h"
#include "nvm.h"


#define DEBOUNCE_TIME      100


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


static void play_press_effect(uint8_t *rgb, uint16_t duration = 300) {
    effect_t pressed = {.type = EFFECT_PULSE, {.pulse = {.rgb = { 0, 0, 0 }, .t_step = 32}}, .duration_ms = duration};
    memcpy(&pressed.pulse.rgb, rgb, sizeof(uint8_t[3]));
    effect_engine_start(pressed, EFFECT_FOREGROUND);
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
            uint8_t rgb[3] = {0, 128, 0};
            play_press_effect(rgb);
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
            uint8_t rgb[3] = {0, 128, 0};
            play_press_effect(rgb);
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
            uint8_t rgb[3] = {0, 128, 0};
            play_press_effect(rgb);
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
            uint8_t rgb[3] = {0, 128, 0};
            play_press_effect(rgb);
        }
    }
    else {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            nvm_config.os = !nvm_config.os;
            nvm_write(nvm_config);
            Serial.println("OS");
            uint8_t rgb[3] = {0, 0, 0};
            if(nvm_config.os == WINDOWS) { rgb[2] = 128; }
            else { rgb[1] = 128; }
            play_press_effect(rgb, 600);
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
            uint8_t rgb[3] = {0, 128, 0};
            play_press_effect(rgb);
        }
    }
    last_press = millis();
}


static void pinMacro3Changed(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            // run_shortcut(UNDO, nvm_config.os);
            // Serial.println("UNDO");
            run_shortcut(ZOOM_MUTE, nvm_config.os);
            Serial.println("ZOOM_MUTE");
            uint8_t rgb[3] = {0, 128, 0};
            play_press_effect(rgb);
        }
    }
    last_press = millis();
}


static void pinModifierChanged() {
    static uint32_t last_press = 0;

    if(digitalRead(pinModifier) && millis() - last_press > DEBOUNCE_TIME) {
        modifier_pressed = true;
        effect_t modifier_effect = {.type = EFFECT_SOLID, {.solid = {.rgb = {16, 16, 16}}}};
        effect_engine_start(modifier_effect, EFFECT_MIDDLE);
    }
    else {
        modifier_pressed = false;
        effect_engine_stop(EFFECT_MIDDLE);
    }

    last_press = millis();
}
