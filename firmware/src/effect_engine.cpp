#include <stdbool.h>
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "effect_engine.h"


static CRGB leds[NUM_LEDS];
static effect_t background_effect;
static effect_t foreground_effect;


extern void effect_engine_init(void) {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);
    FastLED.show();
}


extern void effect_run(effect_t &effect) {
	if(effect.type == EFFECT_BLACKOUT) {
		for(uint8_t i = 0; i < NUM_LEDS; i++) { leds[i] = CRGB(0, 0, 0); }
	}
	else if(effect.type == EFFECT_SOLID) {
		for(uint8_t i = 0; i < NUM_LEDS; i++) { leds[i] = CRGB(effect.color.red, effect.color.green, effect.color.blue); }
	}
	else if(effect.type == EFFECT_HUE_PROGRESS) {
	    static uint8_t hue = 0;

	    hue += 1;
	    for(uint8_t i = 0; i < NUM_LEDS; i++) { leds[i] = CHSV(hue, 255, 255); }

	}
	FastLED.show();
}


extern void effect_engine_run(uint32_t &current_time) {
    static uint32_t last_run_leds = 0;

	if(current_time - last_run_leds > 100) {

		if(foreground_effect.type != EFFECT_NONE) {
			effect_run(foreground_effect);
		}
		else if(background_effect.type != EFFECT_NONE) {
			effect_run(background_effect);
		}

		last_run_leds = current_time;
	}
}


extern void effect_engine_start(effect_t &effect, bool foreground) {
	if(foreground) {
		memcpy(&foreground_effect, &effect, sizeof(effect_t));
	}
	else {
		memcpy(&background_effect, &effect, sizeof(effect_t));
	}
}
