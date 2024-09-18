#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "CH446Q.h"
#include "ConfigMUX.h"

#define EN1_PIN PC2
#define EN2_PIN PC1
#define EN3_PIN PC0
#define USR_BTN PC13

extern Adafruit_NeoPixel strip;

extern std::vector<int> usedMainTrackPins;

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
    delay(100);
    strip.show();
    delay(100); 

    Serial.begin(115200);
    delay(100); 

    setupPMU(true);

    resetMuxes();

    MUX mux1(csPin1, xPins1, yPins1);
    MUX mux2(csPin2, xPins2, yPins2);
    MUX mux3(csPin17, xPins17, yPins17);
    MUX mux4(csPin18, xPins18, yPins18);

    // mux1.printPins();
    // mux2.printPins();

    std::vector<MUX> muxes = {mux1, mux2, mux3, mux4};
    int breadboard1Pin = 1; 
    int breadboard2Pin = 1;

    route(muxes, 1, 1, true);
    route(muxes, 5, 8, true);
    route(muxes, 1, 1, false);
    route(muxes, 1, 1, true);

}

void loop()
{
    // Placeholder for main loop code
}
