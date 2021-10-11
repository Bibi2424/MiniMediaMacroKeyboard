#include <YetAnotherPcInt.h>
#include <HID-Project.h>
#include <FastLED.h>
#include <Encoder.h>

const int pinPlay = 16;
const int pinNext = 10;
const int pinMute = 14;
const int pinMacro1 = 8;
const int pinMacro2 = 15;
const int pinMacro3 = 9;
const int pinModifier = 7;
#define DEBOUNCE_TIME      100

Encoder myEnc(3, 2);

#define LED_PIN     5
#define NUM_LEDS    12
#define BRIGHTNESS  50
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

static bool modifier_pressed = false;


typedef enum {
    COPY = 0,
    PASTE = 1,
    UNDO = 2
} shortcut_t;


#define WINDOWS true
#define MACOS   false
bool selected_os = WINDOWS;


void run_shortcut(shortcut_t shortcut, bool os) {
    switch(shortcut) {
        case COPY:
            Keyboard.press(os ? KEY_LEFT_CTRL: KEY_LEFT_GUI);
            Keyboard.write('c');
            Keyboard.releaseAll();
        break;
        case PASTE:
            Keyboard.press(os ? KEY_LEFT_CTRL: KEY_LEFT_GUI);
            Keyboard.write('v');
            Keyboard.releaseAll();
        break;
        case UNDO:
            Keyboard.press(os ? KEY_LEFT_CTRL: KEY_LEFT_GUI);
            Keyboard.write('z');
            Keyboard.releaseAll();
        break;
    }
}


void setup() {
    delay( 1000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.show();

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
    attachInterrupt(digitalPinToInterrupt(pinModifier), modifier, CHANGE);

    // Sends a clean report to the host. This is important on any Arduino type.
    // Need booth Keyboard and Consumer since they are not capable of sending eachother keycode
    Keyboard.begin();
    Consumer.begin();
}


void pinPlayChanged(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            Consumer.write(MEDIA_PLAY_PAUSE);
        }
    }
    last_press = millis();
}


void pinNextChanged(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            Consumer.write(MEDIA_NEXT);
        }
    }
    last_press = millis();
}


void pinMuteChanged(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            Consumer.write(MEDIA_VOLUME_MUTE);
        }
    }
    last_press = millis();
}


void pinMacro1Changed(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            run_shortcut(COPY, selected_os);
        }
    }
    else {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            selected_os = !selected_os;
        }
    }
    last_press = millis();
}


void pinMacro2Changed(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            run_shortcut(PASTE, selected_os);
        }
    }
    last_press = millis();
}


void pinMacro3Changed(bool new_state) {
    static uint32_t last_press = 0;
    if(modifier_pressed == false) {
        if(new_state == 0 && millis() - last_press > DEBOUNCE_TIME) {
            run_shortcut(UNDO, selected_os);
        }
    }
    last_press = millis();
}


void modifier() {
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
        FastLED.setBrightness(  BRIGHTNESS );
    }

    last_press = millis();
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
    if(current_time - last_run_leds > 100 && !modifier_pressed) {
        hue += 1;
        for(uint8_t i = 0; i < NUM_LEDS; i++) { leds[i] = CHSV(hue, 255, 255); }
        FastLED.show();

        last_run_leds = current_time;
    }
}
