#ifndef CH446Q_H
#define CH446Q_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>

// Define the pin numbers for RST, DAT, STB, and the X and Y address lines
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

#define MAX_X_PINS 16
#define MAX_Y_PINS 8

// Class declaration for the multiplexer (CH446Q)
class MUX
{
private:
    int csPin;                                        // Chip select pin for each individual multiplexer
    std::vector<std::pair<int, int>> usedConnections; // Store used (x, y) pairs
    String xPins[16];                                 // Array for the X address pins
    String yPins[8];                                  // Array for the Y address pins

public:
    MUX(int cs);

    void setConnection(int x, int y, bool mode, int led1, int led2, uint32_t color);

    void setupPins(const String xPinNames[MAX_X_PINS], const String yPinNames[MAX_Y_PINS]);

    void printPins();

    void clearConnections();
};

void resetMuxes();

void setupMUXes(MUX mux1, int numMuxes);

#endif