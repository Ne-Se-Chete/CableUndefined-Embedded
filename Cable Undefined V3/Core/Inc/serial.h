#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"

// Define buffer sizes
#define RX_BUFFER_SIZE 100

void sendToUART(USART_TypeDef *UARTx, const char *message);
void UART_ProcessReceivedByte(uint8_t byte, uint8_t uartNumber);

#endif // SERIAL_H
