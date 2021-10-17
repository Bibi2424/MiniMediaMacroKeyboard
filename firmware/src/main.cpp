#include <Arduino.h>
#include <HID-Project.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Encoder.h>

#include "nvm.h"
#include "button_action.h"
#include "shortcuts.h"
#include "effect_engine.h"


Encoder myEnc(3, 2);


void startup_animation(void) {
    effect_t startup = {.type = EFFECT_PULSE, {.pulse = {.rgb = { 0, 0, 0 }, .t_step = 32}}, .duration_ms = 2000};

    if (nvm_config.os == WINDOWS) { startup.pulse.rgb[2] = 128; }
    else { startup.pulse.rgb[1] = 128; }

    effect_engine_start(startup, EFFECT_FOREGROUND);
}


void setup(void) {
    Serial.begin(115200);

    nvm_read(nvm_config);

    button_init();
    effect_engine_init();

    startup_animation();

    effect_t back = {.type = EFFECT_RAINBOW, {.rainbow = {.brightness = 128, .progression = 4, .offset = 16}}};
    effect_engine_start(back, EFFECT_BACKGROUND);

    // Sends a clean report to the host. This is important on any Arduino type.
    // Need both Keyboard and Consumer since they are not capable of sending eachother keycode
    Keyboard.begin();
    Consumer.begin();

    Serial.println("Init Done");
}


void loop(void) {
    uint32_t current_time = millis();

    static long oldPosition  = 0;
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

    effect_engine_run(current_time);
}
