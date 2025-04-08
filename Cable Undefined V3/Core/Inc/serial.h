#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"

// Define buffer sizes
#define RX_BUFFER_SIZE 100

extern volatile uint8_t usingESP;
extern volatile uint8_t usingCP2102;

void sendRawUART(USART_TypeDef *USARTx, uint8_t *data, uint16_t length);
void sendToUART(USART_TypeDef *UARTx, const char *message);
void UART_ProcessReceivedByte(uint8_t byte, uint8_t uartNumber);

#endif // SERIAL_H
