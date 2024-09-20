#ifndef CH446Q_H
#define CH446Q_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>

// Define the pin numbers for RST, DAT, STB, and the X and Y address lines
#define RST_PIN PB0
#define DAT_PIN PB1
#define STB_PIN PB4

#define AY0_PIN PA0
#define AY1_PIN PA1
#define AY2_PIN PA2 
 
#define AX0_PIN PA3 
#define AX1_PIN PA4 
#define AX2_PIN PB5 
#define AX3_PIN PA6 
 
#define MAX_X_PINS 16 
#define MAX_Y_PINS 8 
#define MAX_TRACK_PINS 32 

#define LED_PIN PB5
#define NUM_LEDS 120

#define EN1_PIN PB6
#define EN2_PIN PB8
#define EN3_PIN PB9
#define FAULT_PIN PB10

struct TrackConnection {
    int trackIndex;
    String pin1Name;
    String pin2Name;
};

extern std::vector<TrackConnection> activeConnections;

// Class declaration for the multiplexer (CH446Q)
class MUX
{
private:
    int csPin;                                        // Chip select pin for each individual multiplexer
    std::vector<std::pair<int, int>> usedConnections; // Store used (x, y) pairs
    String xPins[16];                                 // Array for the X address pins
    String yPins[8];                                  // Array for the Y address pins

public:
    MUX(int cs, const char* xPinNames[MAX_X_PINS], const char* yPinNames[MAX_Y_PINS]);

    void setConnection(int x, int y, bool mode, int led1, int led2, uint32_t color);
    void clearConnections();

    void printPins() const;

    bool findPin(const char* pinName, int &xIndex, int &yIndex) const;
};

void resetMuxes();

void route(std::vector<MUX> &muxes, int breadboardPin1, int breadboardPin2, bool mode, uint32_t color);

void useMainTrack(int trackIndex, const String &pin1Name, const String &pin2Name);
void releaseMainTrack(const String &pin1Name, const String &pin2Name, int &trackIdx);
bool checkAvailableTrack(int &trackIndex);

void setupPMU(bool enable);

#endif