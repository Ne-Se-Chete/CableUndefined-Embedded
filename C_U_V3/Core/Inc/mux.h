#ifndef __MUX_H
#define __MUX_H

#include "main.h"

// Pin definitions for the multiplexer
#define AY0_PIN GPIO_PIN_0
#define AY1_PIN GPIO_PIN_1
#define AY2_PIN GPIO_PIN_2
#define AY_GPIO GPIOB  // Y Address GPIO Port

#define AX0_PIN GPIO_PIN_6
#define AX1_PIN GPIO_PIN_4
#define AX2_PIN GPIO_PIN_3
#define AX3_PIN GPIO_PIN_5
#define AX_GPIO GPIOB  // X Address GPIO Port

#define DAT_PIN GPIO_PIN_7
#define DAT_GPIO GPIOB

#define CS_PIN_1 GPIO_PIN_0
#define CS_PIN_3 GPIO_PIN_2

#define CS_GPIO GPIOC


#define STB_PIN GPIO_PIN_8
#define STB_GPIO GPIOB

#define RST_PIN GPIO_PIN_9
#define RST_GPIO GPIOB

#define DEBUG_PIN_1 GPIO_PIN_12
#define DEBUG_PIN_2 GPIO_PIN_13
#define DEBUG_PIN_3 GPIO_PIN_14
#define DEBUG_PIN_4 GPIO_PIN_15
#define DEBUG_GPIO GPIOB

// Function prototypes
void setConnection(int x, int y,  uint16_t cs, uint8_t mode);

#endif /* __MUX_H */
