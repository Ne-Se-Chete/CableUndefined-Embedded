#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Nordic UART Service (NUS) UUIDs
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"  // Write from central (BLE client to ESP32)
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"  // Notify to central (ESP32 to BLE client)

#define RX_PIN 16  // ESP32 RX pin for UART (TX from STM32)
#define TX_PIN 17  // ESP32 TX pin for UART (RX to STM32)

HardwareSerial UART2(1);  // Hardware UART instance for communication with STM32

BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;

// Callback for BLE connection events
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
    Serial.println("BLE client connected.");
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    Serial.println("BLE client disconnected.");
    // Restart advertising so a new client can connect
    pServer->getAdvertising()->start();
  }
};

// Callback for when data is written by the BLE client (PC)
class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    // Explicit conversion from Arduino String to std::string
    std::string rxValue(pCharacteristic->getValue().c_str());
    if (!rxValue.empty()) {
      Serial.print("Received over BLE: ");
      Serial.println(rxValue.c_str());
      
      // Forward the received BLE data to the STM32 via UART2 using println (adds a newline)
      UART2.println(rxValue.c_str());
      Serial.println("Data sent to STM32 over UART.");
    }
  }
};



void setup() {
  // Start Serial for debug output
  Serial.begin(115200);
  Serial.println("Starting BLE UART Bridge...");

  // Initialize UART2 for STM32 communication
  // Configure at 921600 baud rate, 8 data bits, no parity, 1 stop bit
  UART2.begin(921600, SERIAL_8N1, RX_PIN, TX_PIN);
  
  // Initialize BLE
  BLEDevice::init("Cable Undefined V3 BLE"); // BLE device name
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  // Create BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Create TX characteristic with Notify property
  pTxCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  // Add a descriptor to enable notifications on the client side
  pTxCharacteristic->addDescriptor(new BLE2902());
  
  // Create RX characteristic with Write property (data coming from BLE client)
  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  
  // Start the BLE service
  pService->start();
  
  // Start advertising the BLE service so a central device can discover it.
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  
  Serial.println("BLE advertising started, waiting for client connection...");
}

void loop() {
  // Check if data is available from STM32 via UART2
  while (UART2.available()) {
    uint8_t c = UART2.read();
    Serial.printf("[UART -> BLE] 0x%02X\n", c);

    // If a BLE client is connected, send the received byte as a BLE notification.
    if (deviceConnected) {
      pTxCharacteristic->setValue(&c, 1);
      pTxCharacteristic->notify();
    }
  }
  // Small delay can help with timing.
    delay(10);
}
