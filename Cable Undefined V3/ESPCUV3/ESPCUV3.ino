#include <BluetoothSerial.h>

#define RX_PIN 16  // GPIO16 (RX)
#define TX_PIN 17  // GPIO17 (TX)

// Create Bluetooth Serial object
BluetoothSerial SerialBT;

// Initialize hardware UART (for STM32 communication)
HardwareSerial UART2(1);  

void setup() {
    Serial.begin(115200);    // Debugging on Serial Monitor
    SerialBT.begin("Cable Undefined V3"); // Bluetooth name
    UART2.begin(921600, SERIAL_8N1, RX_PIN, TX_PIN);  // UART for STM32

    Serial.println("ESP32 is ready. Waiting for commands...");
}

void loop() {
    // **Check if Bluetooth sends data**
    if (SerialBT.available()) {
        String btData = SerialBT.readStringUntil('\n');  // Read Bluetooth data
        Serial.println("BT Received: " + btData);
        UART2.println(btData); // Forward to UART2 (STM32)
        Serial.println("Data sent to STM");
    }

    while (UART2.available()) {
        uint8_t c = UART2.read();  // Read raw byte
        Serial.printf("[UART->BT] 0x%02X\n", c);  // Optional debug: show hex
        SerialBT.write(c);  // Forward raw byte to Bluetooth
    }
}
