#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 30

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 7

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {

	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
}

void loop() {
	// Turn the LED on, then pause
//	for (uint16_t counter = 0; counter < NUM_LEDS; ++counter) {
//		leds[counter] = CRGB::Green;
//	}
	leds[0] = CRGB::Green;
	FastLED.show();
	delay(500);
	leds[0] = CRGB::Black;
	leds[1] = CRGB::Orange;
	// Now turn the LED off, then pause
//	for (uint16_t counter = 0; counter < NUM_LEDS; ++counter) {
//		leds[counter] = CRGB::Black;
//	}
	FastLED.show();
	delay(500);
	leds[1] = CRGB::Black;
	leds[2] = CRGB::Green;
	FastLED.show();
	delay(500);
	leds[2] = CRGB::Black;
	leds[3] = CRGB::Orange;
	FastLED.show();
	delay(500);
	leds[3] = CRGB::Black;
	leds[4] = CRGB::Green;
	FastLED.show();
	delay(500);
	leds[4] = CRGB::Black;
	leds[5] = CRGB::Orange;
	FastLED.show();
	delay(500);
	leds[5] = CRGB::Black;
	leds[6] = CRGB::Green;
	FastLED.show();
	delay(500);
	leds[6] = CRGB::Black;
	FastLED.show();

}
