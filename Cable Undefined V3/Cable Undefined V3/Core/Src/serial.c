#include "serial.h"
#include "fault.h"
#include <string.h>
#include <stdio.h>

volatile uint8_t usingESP = 0;
volatile uint8_t usingCP2102 = 0;  // default to start with CP2102

void sendRawUART(USART_TypeDef *USARTx, uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        while (!LL_USART_IsActiveFlag_TXE(USARTx)); // Wait until TX buffer is empty
        LL_USART_TransmitData8(USARTx, data[i]);
    }

    // Optionally wait until transmission is complete
    while (!LL_USART_IsActiveFlag_TC(USARTx));
}

/**
  * @brief  Sends a string over UART.
  */
void sendToUART(USART_TypeDef *UARTx, const char *message)
{
    uint16_t i = 0;

    if (!LL_USART_IsEnabled(UARTx))
        LL_USART_Enable(UARTx);

    while (message[i] != '\0')
    {
        while (!LL_USART_IsActiveFlag_TXE(UARTx));  // Wait for TX buffer empty
        LL_USART_TransmitData8(UARTx, (uint8_t)message[i]);
        i++;
    }

    while (!LL_USART_IsActiveFlag_TC(UARTx));  // Wait for transmission complete


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

        // Trim leading/trailing whitespace
        char* start = (char*)rxBuffer;
        while (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n') start++;

        if (*start != '\0')  // Only print if not empty after trimming
        {
            printf("Received from UART%d: %s\n", uartNumber, start);
            fflush(stdout);
            processCommand(start);
        }

        memset(rxBuffer, 0, RX_BUFFER_SIZE);
        rxIndex = 0;
    }
}



