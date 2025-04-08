// signal_analyzer.c
#include "signal_analyzer.h"
#include "main.h"
#include "serial.h"
#include <stdio.h>
#include <string.h>

static uint16_t adcValues[ADC_CHANNELS];

extern volatile uint8_t usingESP;
extern volatile uint8_t usingCP2102;

ADCChannelConfig adcChannelConfig = {
    .channel_enabled = {0, 0, 0, 0, 0, 0, 0, 0}  // default: all channels enabled
};

volatile SignalMode_t signalMode = SIGNAL_MODE_ADC;  // Default

void OscilloscopeInit(void)
{
    // Configure DMA source/destination and length
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1,
        LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
        (uint32_t)adcValues,
        LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC_CHANNELS);

//    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
//    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
//    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
//    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
//    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
//    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);
//    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH);

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
}

void OscilloscopeDeinit(void)
{
	LL_ADC_REG_StopConversionExtTrig(ADC1);
    LL_ADC_Disable(ADC1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
}

uint32_t getTimestamp(void)
{
    return LL_TIM_GetCounter(TIM6);
}

void sendADCData(void)
{
    uint8_t buffer[32]; // Max: 1 + 1 + (8*2) + 2 = 20 bytes
    uint8_t index = 0;

    buffer[index++] = 0xAA; // Start byte
    buffer[index++] = 0x00; // Reserve space for channel bitmap

    uint8_t channel_map = 0;

    for (int i = 0; i < 8; i++) {
        if (adcChannelConfig.channel_enabled[i]) {
            buffer[index++] = (adcValues[i] >> 8) & 0xFF;
            buffer[index++] = adcValues[i] & 0xFF;
            channel_map |= (1 << i);  // Set bit i if channel is enabled
        }
    }

    // Timestamp (2 bytes, big endian)
    uint16_t t = (uint16_t)getTimestamp();
    buffer[index++] = (t >> 8) & 0xFF;
    buffer[index++] = t & 0xFF;

    // Insert channel map after start byte
    buffer[1] = channel_map;

//  Send via UART
    if (usingESP){
        sendRawUART(USART1, buffer, index);
//    	printf("Sending to ESP");
    }else if (usingCP2102){
        sendRawUART(USART3, buffer, index);
//		printf("Sending to CP2102");
    }

}

void sendDigitalData(void)
{
    uint8_t buffer[16];
    uint8_t index = 0;

    buffer[index++] = 0xAB;  // Different start byte for digital mode
    buffer[index++] = 0x00;  // Reserved for bitmap of active channels

    uint8_t pin_bitmap = 0;

    for (int i = 0; i < 8; i++) {
        if (adcChannelConfig.channel_enabled[i]) {
            uint8_t pin_state = LL_GPIO_IsInputPinSet(GPIOA, i); // Or correct GPIO port
            buffer[index++] = pin_state ? 1 : 0;
            pin_bitmap |= (1 << i);
        }
    }

    // Timestamp
    uint16_t t = (uint16_t)getTimestamp();
    buffer[index++] = (t >> 8) & 0xFF;
    buffer[index++] = t & 0xFF;

    buffer[1] = pin_bitmap;

    if (usingESP)
        sendRawUART(USART1, buffer, index);
    else if (usingCP2102)
        sendRawUART(USART3, buffer, index);
}

void sendSignalData(void)
{
    if (signalMode == SIGNAL_MODE_ADC)
        sendADCData();
    else if (signalMode == SIGNAL_MODE_DIGITAL)
        sendDigitalData();
}



