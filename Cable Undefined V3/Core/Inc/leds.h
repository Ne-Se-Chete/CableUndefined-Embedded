#ifndef LEDS_H
#define LEDS_H

#include "main.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_dma.h"


typedef union
{
  struct
  {
    uint8_t b;
    uint8_t r;
    uint8_t g;
  } color;
  uint32_t data;
} RGB_t;

typedef struct {
    uint8_t pin;
    RGB_t color;
} LedEntry;

#define NEOPIXEL_ZERO 29 	//ZERO = (ARP+1)(0.32)
#define NEOPIXEL_ONE 58		//ONE = (ARP+1)(0.64)
#define NUM_PIXELS 120

#define DMA_BUFF_SIZE (NUM_PIXELS*24)+1

void addToPin(uint8_t pin, RGB_t color);
void removeFromPin(uint8_t pin);

void clearLeds(void);
void sendPixelData();

void LL_TIM_PWM_PulseFinishedCallback(TIM_TypeDef *TIMx);
void PWM_Start_DMA(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t *buffer, uint32_t size);

#endif
