#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Pin definitions for multiplexer control
#define RST_PIN PC12
#define DAT_PIN PC10
#define STB_PIN PC11
#define AX0_PIN PA6
#define AX1_PIN PA7
#define AX2_PIN PB8
#define AX3_PIN PB9
#define AY0_PIN PC3
#define AY1_PIN PC4
#define AY2_PIN PC5
#define CS_PIN PD2

// Pin definitions for PMU control and WS2812B LED strip
#define EN1_PIN PC2
#define EN2_PIN PC1
#define EN3_PIN PC0
#define USR_BTN PC13
#define LED_PIN PF1
#define NUM_LEDS 120

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Function to reset the multiplexer
void resetMUX() {
    digitalWrite(RST_PIN, HIGH);
    delay(100);
    digitalWrite(RST_PIN, LOW);
    delay(100);
}

// Function to select a multiplexer via its chip select line
void selectMultiplexer() {
    digitalWrite(CS_PIN, HIGH);  // Activate the multiplexer (set CS_PIN high)
    delayMicroseconds(10);      // Short delay to ensure CS is stable
}

// Function to set the PMU pins based on the 'enabled' argument
void setupPMU(bool enabled) {
    if (enabled) {
        digitalWrite(EN1_PIN, HIGH);
        digitalWrite(EN2_PIN, HIGH);
        digitalWrite(EN3_PIN, HIGH);
    } else {
        digitalWrite(EN1_PIN, LOW);
        digitalWrite(EN2_PIN, LOW);
        digitalWrite(EN3_PIN, LOW);
    }
    delay(1000);  // Optional delay
}

// Function to set LEDs with a specific color and mode
void setLEDs(uint32_t color, int led1, int led2, bool mode) {
    if (mode) {
        if (led1 >= 0 && led1 < strip.numPixels()) {
            strip.setPixelColor(led1, color);
        }
        if (led2 >= 0 && led2 < strip.numPixels()) {
            strip.setPixelColor(led2, color);
        }
    } else {
        if (led1 >= 0 && led1 < strip.numPixels()) {
            strip.setPixelColor(led1, strip.Color(0, 0, 0)); // Off color
        }
        if (led2 >= 0 && led2 < strip.numPixels()) {
            strip.setPixelColor(led2, strip.Color(0, 0, 0)); // Off color
        }
    }
    strip.show();  // Update the strip
}

// Function to set connection and control LEDs
void setConnection(int x, int y, bool mode, int csPin, int led1, int led2, uint32_t color) {
    // Select the multiplexer
    digitalWrite(csPin, LOW);  // Deactivate the multiplexer (set CS_PIN LOW)
    delayMicroseconds(10);     // Short delay to ensure CS is stable
    digitalWrite(csPin, HIGH); // Activate the multiplexer (set CS_PIN HIGH)

    // Set the X address
    digitalWrite(AX0_PIN, (x & 0x08) == 0x08 ? HIGH : LOW);
    digitalWrite(AX1_PIN, (x & 0x04) == 0x04 ? HIGH : LOW);
    digitalWrite(AX2_PIN, (x & 0x02) == 0x02 ? HIGH : LOW);
    digitalWrite(AX3_PIN, (x & 0x01) == 0x01 ? HIGH : LOW);

    // Set the Y address
    digitalWrite(AY0_PIN, (y & 0x04) == 0x04 ? HIGH : LOW);
    digitalWrite(AY1_PIN, (y & 0x02) == 0x02 ? HIGH : LOW);
    digitalWrite(AY2_PIN, (y & 0x01) == 0x01 ? HIGH : LOW);

    Serial.println("Y:");
    Serial.println((y & 0x04) == 0x04);
    Serial.println((y & 0x02) == 0x02);
    Serial.println((y & 0x01) == 0x01);

    Serial.println("X:");
    Serial.println((x & 0x08) == 0x08);
    Serial.println((x & 0x04) == 0x04);
    Serial.println((x & 0x02) == 0x02);
    Serial.println((x & 0x01) == 0x01);

    // Set the data line based on mode
    digitalWrite(DAT_PIN, mode ? HIGH : LOW);

    // Latch the data to activate the switch
    digitalWrite(STB_PIN, HIGH);
    delayMicroseconds(20);  // Short delay for strobe 
    digitalWrite(STB_PIN, LOW);
    delayMicroseconds(20);  // Short delay for strobe signal

    // Set LEDs with the given parameters
    setLEDs(color, led1, led2, mode);
}

void setup() {
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
    pinMode(CS_PIN, OUTPUT);

    // Set pin modes for PMU control and LED strip
    pinMode(EN1_PIN, OUTPUT);
    pinMode(EN2_PIN, OUTPUT);
    pinMode(EN3_PIN, OUTPUT);
    pinMode(USR_BTN, INPUT_PULLUP);  // Assuming button is active LOW

    // Initialize the LED strip
    strip.begin();           // Initialize the strip
    strip.show();            // Initialize all pixels to 'off'
    Serial.begin(115200);    // Initialize serial communication

    // Initialize PMU
    setupPMU(true);          // Enable the PMU

    // Reset the multiplexer
    resetMUX();
    setConnection(0, 7, true, CS_PIN, 0, 1, strip.Color(255, 0, 0)); // Turn on LEDs 0 and 1 with red color
}

void loop() {

}
