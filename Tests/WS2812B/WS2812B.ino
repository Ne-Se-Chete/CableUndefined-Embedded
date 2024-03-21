#include <FastLED.h>

// First LED strip definitions
#define LED_PIN_1   3
#define NUM_LEDS_1  8

// Second LED strip definitions
#define LED_PIN_2   2   // Example pin for the second strip
#define NUM_LEDS_2  32   // Adjust if your second strip has a different number of LEDs

CRGB leds_1[NUM_LEDS_1];
CRGB leds_2[NUM_LEDS_2];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN_1, GRB>(leds_1, NUM_LEDS_1);
  FastLED.addLeds<WS2812, LED_PIN_2, GRB>(leds_2, NUM_LEDS_2);
}

void loop() {
  // Control the first LED strip
  for(int i = 0; i < NUM_LEDS_1; i++) {
    switch(i % 3) {
      case 0:
        leds_1[i] = CRGB(25, 0, 0); // Red
        break;
      case 1:
        leds_1[i] = CRGB(0, 25, 0); // Green
        break;
      case 2:
        leds_1[i] = CRGB(0, 0, 25); // Blue
        break;
    }
    FastLED.show();
    delay(150); // Adjust this delay to control the speed of the color cycling
  }

  // Control the second LED strip
  for(int i = 0; i < NUM_LEDS_2; i++) {
    switch(i % 3) {
      case 0:
        leds_2[i] = CRGB(25, 25, 0); // Yellow (example color)
        break;
      case 1:
        leds_2[i] = CRGB(0, 25, 25); // Cyan (example color)
        break;
      case 2:
        leds_2[i] = CRGB(25, 0, 25); // Magenta (example color)
        break;
    }
    FastLED.show();
    delay(150); // Adjust this delay to control the speed of the color cycling
  }

  // Optionally, add a delay at the end of the loop to see the pattern hold before it repeats.
  delay(1000); // This delay controls the pause before the pattern repeats.
}
