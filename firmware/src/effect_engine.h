#ifndef EFFECT_ENGINE_H
#define EFFECT_ENGINE_H


#define LED_PIN     		5
#define NUM_LEDS    		12
#define DEFAULT_BRIGHTNESS  50

typedef enum {
	EFFECT_BACKGROUND = 0,
	EFFECT_MIDDLE = 1,
	EFFECT_FOREGROUND = 2,
	EFFECT_NUMBER
} effect_priority_t;

typedef enum {
	EFFECT_NONE = 0,
	EFFECT_BLACKOUT,
	EFFECT_SOLID,
	EFFECT_RAINBOW,
	EFFECT_PULSE
} effect_type_t;

typedef struct {
	uint8_t rgb[3];
} solid_effect_t;

typedef struct {
	uint8_t brightness;
	uint8_t progression;
	uint8_t offset;
	uint8_t _hue;
} rainbow_effect_t;

typedef struct {
	uint8_t rgb[3];
	uint8_t t_step;
	uint8_t x_step;
	uint8_t _t;
} pulse_effect_t;

typedef struct effect {
	effect_type_t type;
	union {
		solid_effect_t solid;
		rainbow_effect_t rainbow;
		pulse_effect_t pulse;
	};
	uint16_t duration_ms;
	uint32_t _start_time_ms;
	bool _first_run;
} effect_t;


extern void effect_engine_init(void);
extern void effect_engine_run(uint32_t &current_time);
extern void effect_engine_start(effect_t &effect, effect_priority_t depth);
extern void effect_engine_stop(effect_priority_t depth);

#endif
