#include "serial.h"
#include "fault.h"
#include <string.h>
#include <stdio.h>


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
        printf("Received from UART%d: %s\n", uartNumber, rxBuffer);
		fflush(stdout);
        processCommand((char *)rxBuffer);  // Pass to processing function

        memset(rxBuffer, 0, RX_BUFFER_SIZE);
        rxIndex = 0;
    }
}


