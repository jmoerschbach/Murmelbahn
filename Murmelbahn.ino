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
uint16_t dot = 0;
void setup() {

	FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
}

void loop() {
	// Turn the LED on, then pause
//	for (uint16_t counter = 0; counter < NUM_LEDS; ++counter) {
//		leds[counter] = CRGB::Green;
//	}
//	for (uint16_t dot = 0; dot < NUM_LEDS; ++dot) {
//		if (dot & 1) {
//			leds[dot] = CRGB::Green;
//		} else {
//			leds[dot] = CRGB::Orange;
//		}
//		FastLED.show();
//		leds[dot] = CRGB::Black;
//		delay(250);
//	}millis()

	EVERY_N_MILLISECONDS(125)
	{
		if (dot & 1 == 1) {
			leds[dot] = CHSV(96, 255, 255);
		} else {
			leds[dot] = CHSV(160, 255, 255);
		}
		FastLED.show();
		fadeToBlackBy(leds, NUM_LEDS, 155);
//		for (uint16_t point = 0; point < NUM_LEDS; ++point) {
//			leds[point].fadeToBlackBy(155);
//		}
		if (dot < NUM_LEDS) {
			++dot;
		} else {
			dot = 0;
		}

	}
}
