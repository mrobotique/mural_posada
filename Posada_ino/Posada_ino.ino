/*
     https://github.com/FastLED/FastLED/wiki
*/


#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    6
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];
//CRGBPalette16 currentPalette; //should look at this later
//TBlendType    currentBlending;//should look at this later

//extern CRGBPalette16 myRedWhiteBluePalette;
//extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void loop() {
  leds[0].setRGB(238, 130, 238);
  leds[0].fadeLightBy(230);
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  leds[1].setRGB(250, 100, 23);
  leds[1].fadeLightBy(230);
  FastLED.show();
  FastLED.delay(10000 / UPDATES_PER_SECOND);
  leds[1].setRGB(0,0,0);
  leds[0].setRGB(0,0,0);
  FastLED.show();
  FastLED.delay(10000 / UPDATES_PER_SECOND);
}
