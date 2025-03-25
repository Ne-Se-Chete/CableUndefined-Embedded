// signal_analyzer.c
#include "signal_analyzer.h"
#include "main.h"
#include "serial.h"
#include <stdio.h>
#include <string.h>

static uint16_t adcValues[ADC_CHANNELS];

void SignalAnalyzer_Init(void)
{
    // Configure DMA source/destination and length
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1,
        LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
        (uint32_t)adcValues,
        LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC_CHANNELS);

    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH);

    // Enable DMA
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    // Enable ADC and wait for stabilization
    LL_ADC_Enable(ADC1);
    LL_mDelay(1);

    // Calibrate ADC
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));

    // Enable ADC DMA transfer
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);

    // Start ADC conversion
    LL_ADC_REG_StartConversionSWStart(ADC1);

    // Start TIM6
    LL_TIM_EnableCounter(TIM6);
}


uint32_t getTimestamp(void)
{
    return LL_TIM_GetCounter(TIM6);
}

void sendADCData(void)
{
    uint32_t timestamp = getTimestamp();
    char buffer[128];

//    buffer[0] = 'A';
//    buffer[1] = '\n';
//    buffer[2] = '\0';
//	  ~55us

// ~1700us
    snprintf(buffer, sizeof(buffer),
             "T:%lu ADC0:%u ADC1:%u ADC2:%u ADC3:%u ADC4:%u ADC5:%u ADC6:%u ADC7:%u\n",
             timestamp, adcValues[0], adcValues[1], adcValues[2], adcValues[3],
             adcValues[4], adcValues[5], adcValues[6], adcValues[7]);

    sendToUART(USART3, buffer); // PC
    sendToUART(USART1, buffer); // ESP32
}
