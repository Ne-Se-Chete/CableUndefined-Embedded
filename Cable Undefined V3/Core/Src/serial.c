#include "serial.h"
#include <string.h>
#include <stdio.h>

/* Private variables */
//static uint8_t rxBuffer[RX_BUFFER_SIZE];
//static uint8_t rxByte;
//static uint8_t rxIndex = 0;


/* Public Variables for TESTING, delete later*/
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t rxByte;
uint8_t rxIndex = 0;

void sendToESP(const char *message)
{
    for (uint16_t i = 0; i < strlen(message); i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART3));
        LL_USART_TransmitData8(USART1, message[i]);
    }
    while (!LL_USART_IsActiveFlag_TC(USART1));
}

void processSerialData(void)
{
    int received = 0;  // Track if we actually received any byte

    if (LL_USART_IsActiveFlag_RXNE(USART1))     // Check UART1 (from ESP32)
    {
        rxByte = LL_USART_ReceiveData8(USART1);
        received = 1; // Mark that data was received
    }
    else if (LL_USART_IsActiveFlag_RXNE(USART3))     // Check UART3 (from PC)
    {
        rxByte = LL_USART_ReceiveData8(USART3);
        received = 1; // Mark that data was received
    }

    // If no data received, exit early
    if (!received) return;

    if (rxIndex < (RX_BUFFER_SIZE - 1))
    {
        rxBuffer[rxIndex++] = rxByte;
    }

    // If newline received, process the command
    if (rxByte == '\n' || rxByte == '\r')
    {
        rxBuffer[rxIndex] = '\0';  // Null-terminate

        // Only print & process if the buffer is not empty
        if (rxIndex > 1)
        {
            printf("Received command: %s\n", rxBuffer);
            processCommand((char *)rxBuffer);  // Pass to processing function
        }

        // Reset buffer for next command
        memset(rxBuffer, 0, RX_BUFFER_SIZE);
        rxIndex = 0;
    }
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
