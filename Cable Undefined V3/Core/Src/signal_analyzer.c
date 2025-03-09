#include "signal_analyzer.h"
#include "serial.h"

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
    // Enable ADC
    LL_ADC_Enable(ADC1);
    LL_mDelay(1);

    // Calibrate ADC before use (optional but recommended)
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1));

    // Ensure ADC is set in continuous mode (if not set in CubeMX)
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);

    // Start ADC conversion
    LL_ADC_REG_StartConversionSWStart(ADC1);

    // Enable TIM6 for timestamping
    LL_TIM_EnableCounter(TIM6);
}


/**
  * @brief  Reads ADC values from channels 0-7.
  */
void readADC(void)
{
    for (int i = 0; i < 8; i++)
    {
        // Set the channel manually (if not in scan mode)
        LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0 + i);

        // Start ADC conversion manually (if continuous mode is not enabled)
        LL_ADC_REG_StartConversionSWStart(ADC1);

        // Wait until conversion is complete
        while (!LL_ADC_IsActiveFlag_EOS(ADC1));

        // Read ADC value
        adcValues[i] = LL_ADC_REG_ReadConversionData12(ADC1);
//        printf("ADCVal[%d]: %d\n", i+1, adcValues[i]);

        // Clear the End of Conversion flag
        LL_ADC_ClearFlag_EOS(ADC1);
    }
//    printf("\n\n");

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

//    printf("T:%lu ADC0:%u ADC1:%u ADC2:%u ADC3:%u ADC4:%u ADC5:%u ADC6:%u ADC7:%u\n",
//                 timestamp, adcValues[0], adcValues[1], adcValues[2], adcValues[3],
//                 adcValues[4], adcValues[5], adcValues[6], adcValues[7]);

    sendToUART(USART3, buffer); // Send to PC
    sendToUART(USART1, buffer); // Send to ESP32

}

