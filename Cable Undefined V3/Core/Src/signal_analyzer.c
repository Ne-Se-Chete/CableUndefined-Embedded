#include "signal_analyzer.h"
#include "stm32f1xx_ll_tim.h"
#include <stdio.h>
#include <string.h>

/* ADC Data Buffer */
static uint16_t adcValues[8];

/**
  * @brief  Initializes the ADC and TIM6.
  */
void SignalAnalyzer_Init(void)
{
    LL_ADC_Enable(ADC1);
    LL_mDelay(1);
    LL_ADC_REG_StartConversionSWStart(ADC1);

    LL_TIM_EnableCounter(TIM6);
}

/**
  * @brief  Reads ADC values from channels 0-7.
  */
void readADC(void)
{
    for (int i = 0; i < 8; i++)
    {
        LL_ADC_REG_SetSequencerRanks(ADC1, i + LL_ADC_REG_RANK_1, i);
        LL_ADC_REG_StartConversionSWStart(ADC1);
        while (!LL_ADC_IsActiveFlag_EOS(ADC1));  // Wait for conversion
        adcValues[i] = LL_ADC_REG_ReadConversionData12(ADC1);
    }
}

/**
  * @brief  Gets current timestamp from TIM6.
//  */
uint32_t getTimestamp(void)
{
    return LL_TIM_GetCounter(TIM6);
}

/**
  * @brief  Sends ADC data via UART.
  */
void sendADCData(void)
{
    readADC();
    uint32_t timestamp = getTimestamp();
    char buffer[100];

    snprintf(buffer, sizeof(buffer), "T:%lu ADC0:%u ADC1:%u ADC2:%u ADC3:%u ADC4:%u ADC5:%u ADC6:%u ADC7:%u\n",
             timestamp, adcValues[0], adcValues[1], adcValues[2], adcValues[3],
             adcValues[4], adcValues[5], adcValues[6], adcValues[7]);

//    sendToUART(USART1, buffer); // Send to ESP32
    sendToUART(USART3, buffer); // Send to PC
}

/**
  * @brief  Sends a string over UART.
  */
void sendToUART(USART_TypeDef *UARTx, const char *message)
{
    for (uint16_t i = 0; i < strlen(message); i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(UARTx));
        LL_USART_TransmitData8(UARTx, message[i]);
    }
    while (!LL_USART_IsActiveFlag_TC(UARTx));
}
