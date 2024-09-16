#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "CH446Q.h"  // Include the multiplexer class

// Pin definitions for PMU control and WS2812B LED strip
#define EN1_PIN PC2
#define EN2_PIN PC1
#define EN3_PIN PC0
#define USR_BTN PC13
#define LED_PIN PF1
#define NUM_LEDS 120

// Initialize LED strip
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Function to set the PMU pins based on the 'enabled' argument
void setupPMU(bool enabled)
{
    if (enabled)
    {
        digitalWrite(EN1_PIN, HIGH);
        digitalWrite(EN2_PIN, HIGH);
        digitalWrite(EN3_PIN, HIGH);
    }
    else
    {
        digitalWrite(EN1_PIN, LOW);
        digitalWrite(EN2_PIN, LOW);
        digitalWrite(EN3_PIN, LOW);
    }
    delay(1000); // Optional delay
}

void setup()
{
    // Set pin modes for multiplexer control lines
    pinMode(RST_PIN, OUTPUT);
    pinMode(DAT_PIN, OUTPUT);
    pinMode(STB_PIN, OUTPUT);
    pinMode(AX0_PIN, OUTPUT);
    pinMode(AX1_PIN, OUTPUT);
    pinMode(AX2_PIN, OUTPUT);
    pinMode(AX3_PIN, OUTPUT);
    pinMode(AY0_PIN, OUTPUT);
    pinMode(AY1_PIN, OUTPUT);
    pinMode(AY2_PIN, OUTPUT);

    // Set pin modes for PMU control and LED strip
    pinMode(EN1_PIN, OUTPUT);
    pinMode(EN2_PIN, OUTPUT);
    pinMode(EN3_PIN, OUTPUT);
    pinMode(USR_BTN, INPUT_PULLUP); // Assuming button is active LOW

    // Initialize the LED strip
    strip.begin();        // Initialize the strip
    strip.show();         // Initialize all pixels to 'off'
    Serial.begin(115200); // Initialize serial communication

    // Initialize PMU
    setupPMU(true); // Enable the PMU

    // Reset and initialize multiplexer
    MUX mux1(PD2);
    MUX mux2(PA0);

    mux1.reset();
    mux1.setConnection(0, 7, true, 0, 1, strip.Color(255, 0, 0)); // Turn on LEDs 0 and 1 with red color

    mux2.reset();
    mux2.setConnection(0, 0, true, 2, 3, strip.Color(0, 255, 0)); // Turn on LEDs 2 and 3 with green color

}

void loop()
{
    // Placeholder for main loop code
}
