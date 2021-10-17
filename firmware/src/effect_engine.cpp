#include <stdbool.h>
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "effect_engine.h"
#include "LUT.h"


static CRGB leds[NUM_LEDS];
static effect_t effects[EFFECT_NUMBER];


extern void effect_engine_init(void) {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);
    FastLED.show();
}


static void effect_run(effect_t &effect) {
    if(effect.type == EFFECT_BLACKOUT) {
        fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
    }
    else if(effect.type == EFFECT_SOLID) {
        fill_solid(leds, NUM_LEDS, CRGB(effect.solid.rgb[0], effect.solid.rgb[1], effect.solid.rgb[2]));
    }
    else if(effect.type == EFFECT_RAINBOW) {
        if(effect._first_run == true) { effect.rainbow._hue = 0; effect._first_run = false; }
        //! Close to FastLED fill_rainbow
        for(uint8_t i = 0; i < NUM_LEDS; i++) {
            leds[i].setHSV(effect.rainbow._hue + i * effect.rainbow.offset, 255, effect.rainbow.brightness);
        }
        effect.rainbow._hue += effect.rainbow.progression;
    }
    else if(effect.type == EFFECT_PULSE) {
        if(effect._first_run == true) { effect.pulse._t = 0; effect._first_run = false; }

        for(uint8_t i = 0; i < NUM_LEDS; i++) {
            // uint8_t scale = triwave8((effect.pulse._t + i * effect.pulse.x_step) & 0xff);
            uint8_t scale = pgm_read_byte_near(triangle_LUT + ((effect.pulse._t + i * effect.pulse.x_step) & 0xff));
            leds[i].setRGB(scale8(effect.pulse.rgb[0], scale), scale8(effect.pulse.rgb[1], scale), scale8(effect.pulse.rgb[2], scale));
        }
        effect.pulse._t += effect.pulse.t_step;
    }
    FastLED.show();
}


extern void effect_engine_run(uint32_t &current_time) {
    static uint32_t last_run_leds = 0;

    if(current_time - last_run_leds > 50) {

        Serial.print("E:");
        for(int8_t i = EFFECT_FOREGROUND; i >= 0; i--) {
            Serial.print(i);
            if(effects[i].type == EFFECT_NONE) { continue; }

            Serial.print("r");
            effect_run(effects[i]);

            if(current_time - effects[i]._start_time_ms > effects[i].duration_ms && effects[i].duration_ms != 0) {
                effects[i].type = EFFECT_NONE;
                // continue;
            }

            break;
        }
        Serial.println("");

        FastLED.show();

        last_run_leds = current_time;
    }
}


extern void effect_engine_start(effect_t &effect, effect_priority_t depth) {
    effect._first_run = true;
    effect._start_time_ms = millis();
    memcpy(&effects[depth], &effect, sizeof(effect_t));
}


extern void effect_engine_stop(effect_priority_t depth) {
    effects[depth].type = EFFECT_NONE;
}

