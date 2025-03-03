#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"

// Define buffer sizes
#define RX_BUFFER_SIZE 100


/* Public Variables for TESTING, delete later*/
extern uint8_t rxBuffer[RX_BUFFER_SIZE];
extern uint8_t rxByte;
extern uint8_t rxIndex;



// Function Prototypes
void sendToESP(const char *message);
void processSerialData(void);

#endif // SERIAL_H
