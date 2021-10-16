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


void setup() {
    Serial.begin(115200);

    nvm_read(nvm_config);

    button_init();
    effect_engine_init();
    effect_t back_effect = {.type = EFFECT_HUE_PROGRESS, .color = {0}, .duration = 0};
    effect_engine_start(back_effect, false);

    // Sends a clean report to the host. This is important on any Arduino type.
    // Need both Keyboard and Consumer since they are not capable of sending eachother keycode
    Keyboard.begin();
    Consumer.begin();

    Serial.println("Init Done");
}


void loop() {
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
