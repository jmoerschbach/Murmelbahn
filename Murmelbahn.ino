#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS_RAILS 105
#define NUM_LEDS_POSTS 34
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN_RIGHT_POST 7
#define DATA_PIN_LEFT_POST 4
#define DATA_PIN_FRONT_RAIL 6
#define DATA_PIN_BACK_RAIL 5
#define BUTTON_PIN 8
#define MARBLE_SPEED 60
#define COUNTDOWN_STARTVALUE 10
// Define the array of leds_front_rail
CRGB leds_front_rail[NUM_LEDS_RAILS];
CRGB leds_back_rail[NUM_LEDS_RAILS];
CRGB leds_right_post[NUM_LEDS_POSTS];
CRGB leds_left_post[NUM_LEDS_POSTS];
uint16_t dot = 0;
uint8_t countdown = COUNTDOWN_STARTVALUE;
uint8_t hue_delta = 5;
uint8_t hue_initial = 0;
//uint8_t current_state = 0;
enum State {
	KNOPF, COUNTDOWN, MOVINGMARBLE, FADEOUT
};
State current_state = KNOPF;

void fade_out() {
	EVERY_N_MILLISECONDS(MARBLE_SPEED)
	{
		fadeToBlackBy(leds_front_rail, NUM_LEDS_RAILS, 155);
		fadeToBlackBy(leds_back_rail, NUM_LEDS_RAILS, 155);
		FastLED.show();
		if (!leds_front_rail[NUM_LEDS_RAILS - 1]) {
			current_state = KNOPF;
		}
	}
}
void button() {
	EVERY_N_MILLISECONDS(100) {
//		for (uint8_t rainbow = 0; rainbow < NUM_LEDS_POSTS; ++rainbow) {
////		leds_left_post[rainbow].
//		}
		fill_rainbow(leds_left_post, NUM_LEDS_POSTS, hue_initial++, hue_delta);
		fill_rainbow(leds_right_post, NUM_LEDS_POSTS, hue_initial++,
				-hue_delta);
		FastLED.show();
	}
	if (digitalRead(BUTTON_PIN) == LOW) {
		current_state = COUNTDOWN;
	}
}
void count_down() {
	EVERY_N_SECONDS(1)
	{
		FastLED.clear();
		int numLedsToLight = map(countdown, 0, COUNTDOWN_STARTVALUE, 0,
		NUM_LEDS_POSTS);
		CRGB color;
		if (countdown > 7) {
			color = CRGB::Red;
		} else if (countdown > 4) {
			color = CRGB::Yellow;
		} else {
			color = CRGB::Green;
		}
		fill_solid(leds_right_post, numLedsToLight, color);
		fill_solid(leds_left_post, numLedsToLight, color);
		FastLED.show();
		if (countdown-- == 0) {
			dot = 0;
			countdown = COUNTDOWN_STARTVALUE;
			current_state = MOVINGMARBLE;
		}
	}
}
void moving_marble() {
	EVERY_N_MILLISECONDS(MARBLE_SPEED)
	{
		if (dot & 1 == 1) {
			leds_front_rail[dot] = CHSV(96, 255, 255);
			leds_back_rail[dot] = CHSV(96, 255, 255);
		} else {
			leds_front_rail[dot] = CHSV(160, 255, 255);
			leds_back_rail[dot] = CHSV(160, 255, 255);
		}
		FastLED.show();
		fadeToBlackBy(leds_front_rail, NUM_LEDS_RAILS, 155);
		fadeToBlackBy(leds_back_rail, NUM_LEDS_RAILS, 155);
		if (dot < NUM_LEDS_RAILS) {
			++dot;
		} else {
			dot = 0;
		}
		if (dot >= NUM_LEDS_RAILS) {
			current_state = FADEOUT;
		}
	}
}
void setup() {

	FastLED.addLeds<WS2812B, DATA_PIN_FRONT_RAIL, GRB>(leds_front_rail,
	NUM_LEDS_RAILS);
	FastLED.addLeds<WS2812B, DATA_PIN_BACK_RAIL, GRB>(leds_back_rail,
	NUM_LEDS_RAILS);
	FastLED.addLeds<WS2812B, DATA_PIN_RIGHT_POST, GRB>(leds_right_post,
	NUM_LEDS_POSTS);
	FastLED.addLeds<WS2812B, DATA_PIN_LEFT_POST, GRB>(leds_left_post,
	NUM_LEDS_POSTS); // GRB ordering is typical
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	FastLED.clear(true);
//	fill_rainbow(leds_left_post, NUM_LEDS_POSTS, 0, hue_delta);
	FastLED.show();
}

void loop() {
	if (current_state == KNOPF) {
		button();
	} else if (current_state == COUNTDOWN) {
		count_down();
	} else if (current_state == MOVINGMARBLE) {
		moving_marble();
	} else if (current_state == FADEOUT) {
		fade_out();
	}

//	moving_marble();
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

}
