#ifndef EFFECT_ENGINE_H
#define EFFECT_ENGINE_H


#define LED_PIN     		5
#define NUM_LEDS    		12
#define DEFAULT_BRIGHTNESS  50


typedef enum {
	EFFECT_NONE = 0,
	EFFECT_BLACKOUT,
	EFFECT_SOLID,
	EFFECT_HUE_PROGRESS
} effect_type_t;

typedef struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} color_t;

typedef struct effect {
	effect_type_t type;
	color_t color;
	uint32_t duration;
} effect_t;


extern void effect_engine_init(void);
extern void effect_engine_run(uint32_t &current_time);
extern void effect_engine_start(effect_t &effect, bool foreground);

#endif
