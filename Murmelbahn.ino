#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS_RAILS 105
#define NUM_LEDS_POSTS 34
// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN_RIGHT_POST 8
#define DATA_PIN_LEFT_POST 6
#define DATA_PIN_FRONT_RAIL 12
#define DATA_PIN_BACK_RAIL 10
#define BUTTON_PIN 4
#define MARBLE_SPEED 58
#define MAX_NUMBER_OF_MARBLES 3
#define NOT_USED NUM_LEDS_RAILS+1
#define COUNTDOWN_STARTVALUE 10
#define BRIGHTNESS 50
// Define the array of leds_front_rail
CRGB leds_front_rail[NUM_LEDS_RAILS];
CRGB leds_back_rail[NUM_LEDS_RAILS];
CRGB leds_right_post[NUM_LEDS_POSTS];
CRGB leds_left_post[NUM_LEDS_POSTS];
uint8_t dots[MAX_NUMBER_OF_MARBLES];
uint16_t dot = 0;
uint16_t dot_post = 0;
uint8_t countdown = COUNTDOWN_STARTVALUE;
uint8_t hue_delta = 5;
uint8_t hue_initial = 0;
uint8_t hue = 0;

boolean direction = false;

//uint8_t current_state = 0;
enum State {
	KNOPF, COUNTDOWN, MOVINGMARBLE, FADEOUT, WAIT, BLINK
};
State current_state = KNOPF;

void fade_out() {
	EVERY_N_MILLISECONDS(MARBLE_SPEED)
	{
		fadeToBlackBy(leds_front_rail, NUM_LEDS_RAILS, 155);
		fadeToBlackBy(leds_back_rail, NUM_LEDS_RAILS, 155);
		FastLED.show();
		if (!leds_front_rail[NUM_LEDS_RAILS - 1]) {
			dot = 0;
			current_state = BLINK;
		}
	}
}
//void button() {
//	EVERY_N_MILLISECONDS(100) {
////		for (uint8_t rainbow = 0; rainbow < NUM_LEDS_POSTS; ++rainbow) {
//////		leds_left_post[rainbow].
////		}
//		fill_rainbow(leds_left_post, NUM_LEDS_POSTS, hue_initial++, hue_delta);
//		fill_rainbow(leds_right_post, NUM_LEDS_POSTS, hue_initial++,
//				-hue_delta);
//		FastLED.show();
//	}
//	if (digitalRead(BUTTON_PIN) == LOW) {
//		current_state = COUNTDOWN;
//	}
//}
//void animation1() {
//	EVERY_N_MILLISECONDS(MARBLE_SPEED)
//	{
//		//		for (uint8_t rainbow = 0; rainbow < NUM_LEDS_POSTS; ++rainbow) {
//		////		leds_left_post[rainbow].
//		//		}
//		if (dot++ >= NUM_LEDS_RAILS) {
//			dot = 0;
//		}
//		leds_front_rail[dot] = CHSV(++hue, 255, 255);
//		leds_back_rail[NUM_LEDS_RAILS - dot] = CHSV(255 - hue, 255, 255);
//		fadeToBlackBy(leds_back_rail, NUM_LEDS_RAILS, 50);
//		fadeToBlackBy(leds_front_rail, NUM_LEDS_RAILS, 50);
//
//		if (dot_post++ >= NUM_LEDS_POSTS) {
//			dot_post = 0;
//			direction = !direction;
//		}
//		leds_left_post[dot_post] = CHSV(++hue, 255, 255);
//		leds_right_post[NUM_LEDS_POSTS - dot_post] = CHSV(255 - hue, 255, 255);
//
//		fadeToBlackBy(leds_left_post, NUM_LEDS_POSTS, 50);
//		fadeToBlackBy(leds_right_post, NUM_LEDS_POSTS, 50);
//		FastLED.show();
//	}
//}
void blink_blink() {
	for (uint8_t i = 0; i < 2; ++i) {
		fill_solid(leds_left_post, NUM_LEDS_POSTS, CHSV(55, 255, 255));
		fill_solid(leds_right_post, NUM_LEDS_POSTS, CHSV(55, 255, 255));
		FastLED.show();
		delay(200);
		FastLED.clear(true);
		delay(200);
	}
	delay(100);
	current_state = KNOPF;
}

void button() {
	EVERY_N_MILLISECONDS(250) {
		leds_front_rail[dot] = CHSV(hue, 255, 255);
		leds_back_rail[NUM_LEDS_RAILS - dot] = CHSV(hue, 255, 255);
		FastLED.show();
		if (dot < NUM_LEDS_RAILS) {
			++dot;
		} else {
			dot = 0;
		}
	}
	EVERY_N_MILLISECONDS(MARBLE_SPEED/2) {
		fill_rainbow(leds_left_post, NUM_LEDS_POSTS, ++hue, 5);
		fill_rainbow(leds_right_post, NUM_LEDS_POSTS, 96 - hue, 5);
		FastLED.show();
	}
	if (digitalRead(BUTTON_PIN) == LOW) {
		current_state = COUNTDOWN;
		countdown = COUNTDOWN_STARTVALUE;
	}
}

void wait() {
	delay(900);
	current_state = MOVINGMARBLE;
}

void activate_new_marble() {

	for (uint8_t i = 0; i < MAX_NUMBER_OF_MARBLES; i++) {
		if (dots[i] == NOT_USED) {
			dots[i] = 0;
			break;
		}
	}
}
boolean no_active_marbles_anymore() {
	for (uint8_t i = 0; i < MAX_NUMBER_OF_MARBLES; ++i) {
		if (dots[i] != NOT_USED) {
			return false;
		}
	}
	return true;
}
void count_down() {
	EVERY_N_MILLISECONDS(1100)
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
//			dot = 0;
			activate_new_marble();
			countdown = COUNTDOWN_STARTVALUE;
			current_state = WAIT;
		}
	}
}
void moving_marble() {
	if (digitalRead(BUTTON_PIN) == LOW) {
		activate_new_marble();
	}
	EVERY_N_MILLISECONDS(MARBLE_SPEED) {
		for (uint8_t i = 0; i < MAX_NUMBER_OF_MARBLES; ++i) {
			if (dots[i] != NOT_USED) {
				if (dots[i] & 1 == 1) {
					leds_front_rail[dots[i]] = CHSV(96, 255, 255);
					leds_back_rail[dots[i]] = CHSV(96, 255, 255);
				} else {
					leds_front_rail[dots[i]] = CHSV(160, 255, 255);
					leds_back_rail[dots[i]] = CHSV(160, 255, 255);
				}
			}
		}
//		if (dot & 1 == 1) {
//			leds_front_rail[dot] = CHSV(96, 255, 255);
//			leds_back_rail[dot] = CHSV(96, 255, 255);
//		} else {
//			leds_front_rail[dot] = CHSV(160, 255, 255);
//			leds_back_rail[dot] = CHSV(160, 255, 255);
//		}
		FastLED.show();
		fadeToBlackBy(leds_front_rail, NUM_LEDS_RAILS, 155);
		fadeToBlackBy(leds_back_rail, NUM_LEDS_RAILS, 155);
		for (uint8_t i = 0; i < MAX_NUMBER_OF_MARBLES; ++i) {
			if (dots[i] != NOT_USED) {
				dots[i] = dots[i] + 1;
				if (dots[i] >= NUM_LEDS_RAILS) {
					dots[i] = NOT_USED;
				}
			}
		}
//		if (dot < NUM_LEDS_RAILS) {
//			++dot;
//		} else {
//			dot = 0;
//		}
//		if (dot >= NUM_LEDS_RAILS) {
//			current_state = FADEOUT;
//		}
	}

	if (no_active_marbles_anymore()) {
		current_state = FADEOUT;
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
	FastLED.setBrightness(BRIGHTNESS);
	FastLED.clear(true);
//	fill_rainbow(leds_left_post, NUM_LEDS_POSTS, 0, hue_delta);
	FastLED.show();

	for (uint8_t i = 0; i < MAX_NUMBER_OF_MARBLES; ++i) {
		dots[i] = NOT_USED;
	}
}

void loop() {
	if (current_state == KNOPF) {
		button();
	} else if (current_state == WAIT) {
		wait();
	} else if (current_state == COUNTDOWN) {
		count_down();
	} else if (current_state == MOVINGMARBLE) {
		moving_marble();
	} else if (current_state == FADEOUT) {
		fade_out();
	} else if (current_state == BLINK) {
		blink_blink();
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
