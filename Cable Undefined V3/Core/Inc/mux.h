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

#define STB_PIN GPIO_PIN_8
#define STB_GPIO GPIOB

#define RST_PIN GPIO_PIN_9
#define RST_GPIO GPIOB

#define DEBUG_PIN_1 GPIO_PIN_12
#define DEBUG_PIN_2 GPIO_PIN_13
#define DEBUG_PIN_3 GPIO_PIN_14
#define DEBUG_PIN_4 GPIO_PIN_15
#define DEBUG_GPIO GPIOB

typedef struct {
    int track_id;
    int net_id;
    int is_used;
} MainTrack;

extern MainTrack mainTracks[32];

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    const char* const (*xPins)[16];  // Pointer to an array of 16 strings
    const char* const (*yPins)[8];   // Pointer to an array of 8 strings
} MUX;

extern const MUX muxes[34];

void setConnection(int x, int y, MUX mux, uint8_t mode);

void route(int breadboardPin1, int breadboardPin2, int net_id, MUX *muxes, size_t muxCount, uint8_t mode);

#endif /* __MUX_H */
