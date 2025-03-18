#include "leds.h"

RGB_t pixel[NUM_PIXELS] = {0};
uint32_t dmaBuffer[DMA_BUFF_SIZE] = {0};

LedEntry LedList[NUM_PIXELS];
uint8_t numActiveLeds = 0;


void addToPin(uint8_t pin, RGB_t color) {
    if (pin == 0 || pin > NUM_PIXELS || numActiveLeds >= NUM_PIXELS) {
        return;
    }

    // Update if pin already exists
    for (uint8_t i = 0; i < numActiveLeds; i++) {
        if (LedList[i].pin == pin) {
            LedList[i].color = color;
            return;
        }
    }

    // Add new entry
    LedList[numActiveLeds].pin = pin;
    LedList[numActiveLeds].color = color;
    numActiveLeds++;
}

void removeFromPin(uint8_t pin) {
    for (uint8_t i = 0; i < numActiveLeds; i++) {
        if (LedList[i].pin == pin) {
            for (uint8_t j = i; j < numActiveLeds - 1; j++) {
                LedList[j] = LedList[j + 1];
            }
            numActiveLeds--;
            return;
        }
    }
}



void clearBoard(void) {
    uint32_t *pBuff = dmaBuffer;
    for (int i = 0; i < NUM_PIXELS * 24; i++) {
        *pBuff++ = NEOPIXEL_ZERO;
    }

    dmaBuffer[DMA_BUFF_SIZE - 1] = 0;

    PWM_Start_DMA(TIM2, LL_TIM_CHANNEL_CH1, dmaBuffer, DMA_BUFF_SIZE);
    LL_mDelay(10);
}

void sendPixelData() {
    LL_mDelay(10);
    if (numActiveLeds == 0) {
        clearBoard();
        return;
    }

    // Reset pixel buffer
//    for (int i = 0; i < NUM_PIXELS; i++) {
//        pixel[i].data = 0;
//    }

    // Copy assigned LED colors
    // Copy assigned LED colors
    for (uint8_t i = 0; i < numActiveLeds; i++) {
        uint8_t pin = LedList[i].pin;
        if (pin > 0 && pin <= NUM_PIXELS) {
            pixel[pin - 1] = LedList[i].color;
            pixel[pin - 1].color.r >>= 2;
            pixel[pin - 1].color.g >>= 2;
            pixel[pin - 1].color.b >>= 2;
        }
    }



    // Transfer pixel data into DMA buffer
    uint32_t *pBuff = dmaBuffer;
    for (int i = 0; i < NUM_PIXELS; i++) {
    	if (pixel[i].data != 0) {
    	//        	        printf("Buffer[%d]: %d\n", (i * 24) + (23 - j), bit); // Prints every bit written to the buffer
    		printf("LED %d - R: ", i + 1);
			for (int j = 7; j >= 0; j--) {
				uint32_t bit = (pixel[i].color.r >> j) & 0x01;
				printf("%d", bit);
			}
			printf("  G: ");
			for (int j = 7; j >= 0; j--) {
				uint32_t bit = (pixel[i].color.g >> j) & 0x01;
				printf("%d", bit);
			}
			printf("  B: ");
			for (int j = 7; j >= 0; j--) {
				uint32_t bit = (pixel[i].color.b >> j) & 0x01;
				printf("%d", bit);
			}
			printf("\n");
		}
        for (int j = 23; j >= 0; j--) {
        	uint32_t bit = (pixel[i].data >> j) & 0x01 ? NEOPIXEL_ONE : NEOPIXEL_ZERO;

        	*pBuff++ = bit;
//        	dmaBuffer[i * 24 + j] = bit;
        }
    }

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
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);

	LL_TIM_CC_DisableChannel(TIMx, LL_TIM_CHANNEL_CH1);
	LL_TIM_DisableCounter(TIMx);
}
