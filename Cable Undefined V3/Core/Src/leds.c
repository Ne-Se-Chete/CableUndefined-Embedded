#include "leds.h"

RGB_t pixel[NUM_PIXELS] = {0};
uint32_t dmaBuffer[DMA_BUFF_SIZE] = {0};

LedPair LedPairs[NUM_PIXELS/2];
uint8_t numLedPairs = 0;


void addToPins(uint8_t pin1, uint8_t pin2, RGB_t color) {
    if (pin1 == pin2 || pin1 == 0 || pin2 == 0 || pin1 >= NUM_PIXELS+1 || pin2 >= NUM_PIXELS+1 || numLedPairs >= NUM_PIXELS/2) {
        return;
    }

    LedPairs[numLedPairs].pin1 = pin1;
    LedPairs[numLedPairs].pin2 = pin2;
    LedPairs[numLedPairs].color = color;
    numLedPairs++;
}

void removeFromPins(uint8_t pin1, uint8_t pin2) {
    if (pin1 == pin2 || pin1 == 0 || pin2 == 0 || pin1 >= NUM_PIXELS+1 || pin2 >= NUM_PIXELS+1) {
        return;
    }

    for (uint8_t i = 0; i < numLedPairs; i++) {
        if ((LedPairs[i].pin1 == pin1 && LedPairs[i].pin2 == pin2) ||
            (LedPairs[i].pin1 == pin2 && LedPairs[i].pin2 == pin1)) {

            for (uint8_t j = i; j < numLedPairs - 1; j++) {
                LedPairs[j] = LedPairs[j + 1];
            }

            numLedPairs--;
            return;
        }
    }
}


void clearBoard(void) {
    int i;
    uint32_t *pBuff = dmaBuffer;
    for (i = 0; i < NUM_PIXELS; i++) {
        for (int j = 23; j >= 0; j--) {
            *pBuff++ = NEOPIXEL_ZERO;
        }
    }

    dmaBuffer[DMA_BUFF_SIZE - 1] = 0;

    PWM_Start_DMA(TIM2, LL_TIM_CHANNEL_CH1, dmaBuffer, DMA_BUFF_SIZE);
    LL_mDelay(10);
}

void sendPixelData() {
    if (numLedPairs == 0) {
    	clearBoard();
        return;
    }

    int i, j;
    uint32_t *pBuff = dmaBuffer;

    // Reset all pixels
    for (i = 0; i < NUM_PIXELS; i++) {
        pixel[i].data = 0;  // Turn off all pixels initially
    }

    for (i = 0; i < numLedPairs; i++) {
	   uint8_t pin1 = LedPairs[i].pin1;
	   uint8_t pin2 = LedPairs[i].pin2;
	   RGB_t color = LedPairs[i].color;

	   if (pin1 > 0 && pin1 <= NUM_PIXELS) {
		   pixel[pin1 - 1] = color;  // Set the color for pin1
		   pixel[pin1 - 1].color.r >>= 2;
		   pixel[pin1 - 1].color.g >>= 2;
		   pixel[pin1 - 1].color.b >>= 2;
	   }

	   if (pin2 > 0 && pin2 <= NUM_PIXELS) {
		   pixel[pin2 - 1] = color;  // Set the color for pin2
		   pixel[pin2 - 1].color.r >>= 2;
		   pixel[pin2 - 1].color.g >>= 2;
		   pixel[pin2 - 1].color.b >>= 2;
	   }
   }


    // Transfer data to DMA buffer
    for (i = 0; i < NUM_PIXELS; i++) {
        for (j = 23; j >= 0; j--) {
            *pBuff++ = (pixel[i].data >> j) & 0x01 ? NEOPIXEL_ONE : NEOPIXEL_ZERO;
        }
    }

    // The last element should be 0
    dmaBuffer[DMA_BUFF_SIZE - 1] = 0;

    PWM_Start_DMA(TIM2, LL_TIM_CHANNEL_CH1, dmaBuffer, DMA_BUFF_SIZE);
	LL_mDelay(10);
}





void PWM_Start_DMA(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t *buffer, uint32_t size)
{
    // 1. Configure DMA source and destination
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_5,
                           (uint32_t)buffer,                      // Memory Address (source)
                           (uint32_t)&TIMx->CCR1,                 // Peripheral Address (destination)
                           LL_DMA_DIRECTION_MEMORY_TO_PERIPH);    // Memory-to-Peripheral

    // 2. Set number of data items
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, size);

    // 3. Enable DMA Transfer Complete Interrupt (optional)
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);

    // 4. Enable DMA Stream
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);

    // 5. Enable TIM2 DMA request for CC1
    LL_TIM_EnableDMAReq_CC1(TIMx);

    // 6. Enable TIM2 Channel 1 (PWM mode must already be configured)
    LL_TIM_CC_EnableChannel(TIMx, Channel);

    // 7. Start Timer
    LL_TIM_EnableCounter(TIMx);
}

void LL_TIM_PWM_PulseFinishedCallback(TIM_TypeDef *TIMx)
{
	LL_TIM_DisableDMAReq_CC1(TIMx);
	LL_TIM_CC_DisableChannel(TIMx, LL_TIM_CHANNEL_CH1);
	LL_TIM_DisableCounter(TIMx);
}
