#include <HID-Project.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Encoder.h>

#include "nvm.h"
#include "button_action.h"
#include "shortcuts.h"


#define LED_PIN     5
#define NUM_LEDS    12
#define BRIGHTNESS  50
CRGB leds[NUM_LEDS];

Encoder myEnc(3, 2);


void setup() {
    Serial.begin(115200);

    nvm_read(nvm_config);

    button_init();

    // Sends a clean report to the host. This is important on any Arduino type.
    // Need booth Keyboard and Consumer since they are not capable of sending eachother keycode
    Keyboard.begin();
    Consumer.begin();

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.show();

    Serial.println("Init Done");
}


void loop() {
    static uint32_t last_run_leds = 0;

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

    static uint8_t hue = 0;
    if(current_time - last_run_leds > 100 && !is_modifier_pressed()) {
        hue += 1;
        for(uint8_t i = 0; i < NUM_LEDS; i++) { leds[i] = CHSV(hue, 255, 255); }
        FastLED.show();

        last_run_leds = current_time;
    }
}
