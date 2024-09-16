#include "CH446Q.h"

// Global variable for LED strip (this can stay in the main .ino file if preferred)
extern Adafruit_NeoPixel strip;

// Constructor implementation
MUX::MUX(int cs) : csPin(cs) {
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, LOW);  // Initialize the multiplexer as deselected (LOW)
}

// Reset the multiplexer
void MUX::reset() {
    digitalWrite(RST_PIN, HIGH);
    delay(100);
    digitalWrite(RST_PIN, LOW);
    delay(100);

    // Clear the used connections when reset
    usedConnections.clear();
}

// Set the X and Y addresses and send data to the multiplexer
void MUX::setConnection(int x, int y, bool mode, int led1, int led2, uint32_t color) {
    // Validate the x and y values
    if (x < 0 || x > 15 || y < 0 || y > 7) {
        Serial.println("Error: Invalid (x, y) values. x must be between 0-15, and y between 0-7.");
        return;  // Exit if invalid values
    }

    // Handle the mode: true for setting connection, false for removing
    if (mode) {
        // Check if the (x, y) combination is already used
        for (const auto& connection : usedConnections) {
            if (connection.first == x && connection.second == y) {
                Serial.println("Error: This (x, y) combination is already used.");
                return;  // Exit if the (x, y) is already in use
            }
        }

        // If the combination is valid and unused, add it to the list
        usedConnections.push_back({x, y});
        Serial.println("Connection set successfully.");
    } else {
        // If mode is false, remove the (x, y) combination from the list
        for (auto it = usedConnections.begin(); it != usedConnections.end(); ++it) {
            if (it->first == x && it->second == y) {
                usedConnections.erase(it);  // Remove the (x, y) combination
                Serial.println("Connection removed successfully.");
                break;
            }
        }
    }

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

    // Set the data line based on mode
    digitalWrite(DAT_PIN, mode ? HIGH : LOW);

    // Latch the data to activate the switch
    digitalWrite(STB_PIN, HIGH);
    delayMicroseconds(20); // Short delay for strobe
    digitalWrite(STB_PIN, LOW);
    delayMicroseconds(20); // Short delay for strobe signal

    // Set LEDs with the given parameters
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
