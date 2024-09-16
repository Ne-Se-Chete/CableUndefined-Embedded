#ifndef CH446Q_H
#define CH446Q_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>

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

// Class declaration for the multiplexer (CH446Q)
class MUX {
private:
    int csPin;  // Chip select pin for each individual multiplexer
    std::vector<std::pair<int, int>> usedConnections; // Store used (x, y) pairs

public:
    // Constructor
    MUX(int cs);

    // Reset the multiplexer
    void reset();

    // Set the X and Y addresses and send data to the multiplexer
    void setConnection(int x, int y, bool mode, int led1, int led2, uint32_t color);
};

#endif
