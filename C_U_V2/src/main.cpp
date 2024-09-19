#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "CH446Q.h"
#include "ConfigMUX.h"

#define EN1_PIN PB6
#define EN2_PIN PB8
#define EN3_PIN PB9
#define FAULT_PIN PB10

extern Adafruit_NeoPixel strip;

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

    route(muxes, 7, 3, true);
    delay(10000);
    route(muxes, 7, 3, false);
    

    // mux1.setConnection(0, 0, true, 0, 1, strip.Color(255, 255, 0));

}

void loop()
{
    // Placeholder for main loop code
}
