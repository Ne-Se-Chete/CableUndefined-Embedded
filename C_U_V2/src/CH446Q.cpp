#include "CH446Q.h"

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

std::vector<TrackConnection> activeConnections;

MUX::MUX(int csPin, const char **xPinArray, const char **yPinArray)
    : csPin(csPin)
{
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, LOW);

    // Copy xPins
    for (int i = 0; i < 16; ++i)
    {
        xPins[i] = xPinArray[i];
    }

    // Copy yPins
    for (int i = 0; i < 8; ++i)
    {
        yPins[i] = yPinArray[i];
    }
}

void MUX::printPins() const
{
    Serial.print("X Pins: ");
    for (int i = 0; i < 16; ++i)
    {
        if (xPins[i] != nullptr)
        {
            Serial.print(xPins[i]);
        }
        else
        {
            Serial.print("NULL");
        }
        Serial.print(" ");
    }
    Serial.println(); // Newline after printing X pins

    Serial.print("Y Pins: ");
    for (int i = 0; i < 8; ++i)
    {
        if (yPins[i] != nullptr)
        {
            Serial.print(yPins[i]);
        }
        else
        {
            Serial.print("NULL");
        }
        Serial.print(" ");
    }
    Serial.println(); // Newline after printing Y pins
}

void MUX::clearConnections()
{
    usedConnections.clear();
}

void MUX::setConnection(int x, int y, bool mode, int led1, int led2, uint32_t color)
{
    // Validate the x and y values
    if (x < 0 || x > 15 || y < 0 || y > 7)
    {
        Serial.println("Error: Invalid (x, y) values. x must be between 0-15, and y between 0-7.");
        return; // Exit if invalid values
    }

    // Handle the mode: true for setting connection, false for removing
    if (mode)
    {
        // Check if the (x, y) combination is already used
        for (const auto &connection : usedConnections)
        {
            if (connection.first == x && connection.second == y)
            {
                Serial.println("Error: This (x, y) combination is already used.");
                return; // Exit if the (x, y) is already in use
            }
        }

        // If the combination is valid and unused, add it to the list
        usedConnections.push_back({x, y});
        Serial.println("Connection set successfully.");
    }
    else
    {
        // If mode is false, remove the (x, y) combination from the list
        for (auto it = usedConnections.begin(); it != usedConnections.end(); ++it)
        {
            if (it->first == x && it->second == y)
            {
                usedConnections.erase(it); // Remove the (x, y) combination
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

    Serial.print("y: ");
    Serial.print(y & 0x04 == 0x04 ? HIGH : LOW);
    Serial.print(y & 0x02 == 0x02 ? HIGH : LOW);
    Serial.print(y & 0x01 == 0x01 ? HIGH : LOW);

    Serial.print("\n");

    Serial.print("x: ");
    Serial.print(x & 0x08 == 0x08 ? HIGH : LOW);
    Serial.print(x & 0x04 == 0x04 ? HIGH : LOW);
    Serial.print(x & 0x02 == 0x02 ? HIGH : LOW);
    Serial.print(x & 0x01 == 0x01 ? HIGH : LOW);

    Serial.print("\n");

    // Set the data line based on mode
    digitalWrite(DAT_PIN, mode ? HIGH : LOW);

    // Latch the data to activate the switch
    digitalWrite(STB_PIN, HIGH);
    delayMicroseconds(20); // Short delay for strobe
    digitalWrite(STB_PIN, LOW);
    delayMicroseconds(20); // Short delay for strobe signal

    // Set LEDs with the given parameters
    if (mode)
    {
        if (led1 >= 0 && led1 < strip.numPixels())
        {
            strip.setPixelColor(led1, color);
        }
        if (led2 >= 0 && led2 < strip.numPixels())
        {
            strip.setPixelColor(led2, color);
        }
    }
    else
    {
        if (led1 >= 0 && led1 < strip.numPixels())
        {
            strip.setPixelColor(led1, strip.Color(0, 0, 0)); // Off color
        }
        if (led2 >= 0 && led2 < strip.numPixels())
        {
            strip.setPixelColor(led2, strip.Color(0, 0, 0)); // Off color
        }
    }
    strip.show(); // Update the strip
}

bool MUX::findPin(const char *pinName, int &xIndex, int &yIndex) const
{
    // Serial.print("Searching for pin: ");
    // Serial.println(pinName);

    for (int i = 0; i < MAX_X_PINS; ++i)
    {
        if (strcmp(xPins[i].c_str(), pinName) == 0)
        {
            xIndex = i;
            yIndex = -1;
            // Serial.print("Pin found in xPins at index: ");
            // Serial.println(i);
            return true;
        }
    }

    for (int i = 0; i < MAX_Y_PINS; ++i)
    {

        if (strcmp(yPins[i].c_str(), pinName) == 0)
        {
            yIndex = i;
            xIndex = -1;
            // Serial.print("Pin found in yPins at index: ");
            // Serial.println(i);
            return true;
        }
    }

    return false;
}

void releaseMainTrack(const String &pin1Name, const String &pin2Name, int &trackIdx) // Pass by reference
{
    auto it = std::find_if(activeConnections.begin(), activeConnections.end(),
                           [&](const TrackConnection &connection)
                           {
                               return connection.pin1Name == pin1Name && connection.pin2Name == pin2Name;
                           });

    if (it != activeConnections.end())
    {
        int trackIndex = it->trackIndex; // Get the correct track index        
        activeConnections.erase(it);     // Remove the track from activeConnections
        trackIdx = trackIndex;           // Assign trackIndex to the reference

        Serial.print("Released track ");
        Serial.print(trackIndex);
        Serial.print(" for pins ");
        Serial.print(pin1Name);
        Serial.print(" and ");
        Serial.println(pin2Name);
    }
    else
    {
        Serial.println("Error: No matching track found for the provided pins.");
        trackIdx = -1; // Optionally, set trackIdx to a default value if no track is found
    }
}


void useMainTrack(int trackIndex, const String &pin1Name, const String &pin2Name)
{
    if (trackIndex >= 0 && trackIndex < MAX_TRACK_PINS)
    {
        // Store the track and associated pins in the global vector
        TrackConnection connection = {trackIndex, pin1Name, pin2Name};
        activeConnections.push_back(connection);
        Serial.print("Using track ");
        Serial.print(trackIndex);
        Serial.print(" for pins ");
        Serial.print(pin1Name);
        Serial.print(" and ");
        Serial.println(pin2Name);
    }
}

bool checkAvailableTrack(int &trackIndex)
{
    for (int i = 0; i < MAX_TRACK_PINS; ++i)
    {
        int currentIndex = i; // Direct index since we're iterating over all tracks

        // Check if the currentIndex is already used in activeConnections
        bool trackInUse = false;
        for (const auto &connection : activeConnections)
        {
            if (connection.trackIndex == currentIndex)
            {
                trackInUse = true; // Track is already used
                break;
            }
        }

        // If track is not in use, assign it to trackIndex and return true
        if (!trackInUse)
        {
            trackIndex = currentIndex;
            return true;
        }
    }

    return false; // No available track pins found
}

void route(std::vector<MUX> &muxes, int breadboardPin1, int breadboardPin2, bool mode)
{
    String pin1Name = "B1_" + String(breadboardPin1);
    String pin2Name = "B2_" + String(breadboardPin2);

    int xIndex1 = -1, yIndex1 = -1;
    int xIndex2 = -1, yIndex2 = -1;
    int trackIndex = -1;

    bool pin1Found = false, pin2Found = false;
    MUX *mux1 = nullptr;
    MUX *mux2 = nullptr;

    // Find the pins in the MUXes
    for (size_t i = 0; i < muxes.size(); ++i)
    {
        if (muxes[i].findPin(pin1Name.c_str(), xIndex1, yIndex1))
        {
            // Serial.print("xIndex1: ");
            // Serial.println(xIndex1);
            // Serial.print("yIndex1: ");
            // Serial.println(yIndex1);

            pin1Found = true;
            mux1 = &muxes[i];
            break;
        }
    }

    for (size_t j = 0; j < muxes.size(); ++j)
    {
        if (muxes[j].findPin(pin2Name.c_str(), xIndex2, yIndex2))
        {
            // Serial.print("xIndex2: ");
            // Serial.println(xIndex2);
            // Serial.print("yIndex2: ");
            // Serial.println(yIndex2);

            pin2Found = true;
            mux2 = &muxes[j];
            break;
        }
    }

    // If both pins are found
    if (pin1Found && pin2Found)
    {
        if (mode) // Set track
        {

            if (checkAvailableTrack(trackIndex))
            {
                useMainTrack(trackIndex, pin1Name, pin2Name);

                int _;

                String connectedTrackName = "MT_" + String(trackIndex + 1);
                Serial.print("trackName: ");
                Serial.println(connectedTrackName);

                mux1->findPin(connectedTrackName.c_str(), xIndex1, _);

                Serial.print("MUX1 x: ");
                Serial.println(xIndex1);
                Serial.print("MUX1 y: ");
                Serial.println(yIndex1);

                mux2->findPin(connectedTrackName.c_str(), xIndex2, _);

                Serial.print("MUX2 x: ");
                Serial.println(xIndex2);
                Serial.print("MUX2 y: ");
                Serial.println(yIndex2);

                // mux1->setConnection(xIndex1, yIndex1, true, trackIndex, -1, strip.Color(255, 255, 0));
            }
            else
            {
                Serial.println("Error: No available main track pins.");
            }
        }
        else // Release track
        {
            int trackIdx;
            releaseMainTrack(pin1Name, pin2Name, trackIdx);
            Serial.print("Releasing: ");
            int _;

            Serial.print("trackIdx: ");
            Serial.println(trackIdx);

            String releasedtrackName = "MT_" + String(trackIdx + 1);
            Serial.print("trackName: ");
            Serial.println(releasedtrackName);
            
            

            mux1->findPin(releasedtrackName.c_str(), xIndex1, _);

            Serial.print("MUX1 x: ");
            Serial.println(xIndex1);
            Serial.print("MUX1 y: ");
            Serial.println(yIndex1);

            mux2->findPin(releasedtrackName.c_str(), xIndex2, _);

            Serial.print("MUX2 x: ");
            Serial.println(xIndex2);
            Serial.print("MUX2 y: ");
            Serial.println(yIndex2);
        }
    }
    else
    {
        if (!pin1Found)
            Serial.println("Error: Pin 1 not found in any MUX.");
        if (!pin2Found)
            Serial.println("Error: Pin 2 not found in any MUX.");
    }
}

void resetMuxes()
{
    digitalWrite(RST_PIN, HIGH);
    delay(100);
    digitalWrite(RST_PIN, LOW);
    delay(100);
}