#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "CH446Q.h" 
#include "ConfigMUX.h" 

#define EN1_PIN PC2
#define EN2_PIN PC1
#define EN3_PIN PC0
#define USR_BTN PC13
#define LED_PIN PF1
#define NUM_LEDS 120

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

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
    delay(1000); 
}

void setup()
{
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

    pinMode(EN1_PIN, OUTPUT);
    pinMode(EN2_PIN, OUTPUT);
    pinMode(EN3_PIN, OUTPUT);
    pinMode(USR_BTN, INPUT_PULLUP); 

    strip.begin();        
    strip.show();         
    Serial.begin(115200);

    setupPMU(true);

    resetMuxes();
    MUX mux1(PD2);
    // MUX mux2(PA0);

    Serial.println("Setting up MUXes");
    mux1.setupPins(xPins1, yPins1); // Setup the pins for the multiplexer
    mux1.printPins(); // Print the pin assignments for the multiplexer

    Serial.println("dun");



}

void loop()
{
    // Placeholder for main loop code
}
