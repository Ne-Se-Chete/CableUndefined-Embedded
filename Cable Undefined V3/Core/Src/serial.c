#include "serial.h"
#include <string.h>
#include <stdio.h>


void sendToESP(const char *message)
{
    for (uint16_t i = 0; i < strlen(message); i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART3));
        LL_USART_TransmitData8(USART1, message[i]);
    }
    while (!LL_USART_IsActiveFlag_TC(USART1));
}


void UART_ProcessReceivedByte(uint8_t byte, uint8_t uartNumber)
{
    static uint8_t rxBuffer[RX_BUFFER_SIZE];
    static uint8_t rxIndex = 0;

    if (rxIndex < (RX_BUFFER_SIZE - 1))
    {
        rxBuffer[rxIndex++] = byte;
    }

    if (byte == '\n' || byte == '\r')
    {
        rxBuffer[rxIndex] = '\0';  // Null-terminate
        printf("Received from UART%d: %s\n", uartNumber, rxBuffer);
		fflush(stdout);
        processCommand((char *)rxBuffer);  // Pass to processing function

        memset(rxBuffer, 0, RX_BUFFER_SIZE);
        rxIndex = 0;
    }
}
