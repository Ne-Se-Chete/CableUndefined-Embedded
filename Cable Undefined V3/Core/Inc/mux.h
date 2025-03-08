#ifndef __MUX_H
#define __MUX_H

#include "main.h"
#include "stm32f1xx_ll_gpio.h"  // Change f4 to your STM32 series if needed
#include <stddef.h>  // Defines size_t

// Pin definitions for the multiplexer
#define AY0_PIN LL_GPIO_PIN_0
#define AY1_PIN LL_GPIO_PIN_1
#define AY2_PIN LL_GPIO_PIN_2
#define AY_GPIO GPIOB  // Y Address GPIO Port

#define AX0_PIN LL_GPIO_PIN_6
#define AX1_PIN LL_GPIO_PIN_4
#define AX2_PIN LL_GPIO_PIN_3
#define AX3_PIN LL_GPIO_PIN_5
#define AX_GPIO GPIOB  // X Address GPIO Port

#define DAT_PIN LL_GPIO_PIN_7
#define DAT_GPIO GPIOB

#define STB_PIN LL_GPIO_PIN_8
#define STB_GPIO GPIOB

#define RST_PIN LL_GPIO_PIN_9
#define RST_GPIO GPIOB

#define DEBUG_PIN_1 LL_GPIO_PIN_12
#define DEBUG_PIN_2 LL_GPIO_PIN_13
#define DEBUG_PIN_3 LL_GPIO_PIN_14
#define DEBUG_PIN_4 LL_GPIO_PIN_15
#define DEBUG_GPIO GPIOB

typedef struct {
    int track_id;
    int net_id;
    int is_used;
    int current_connections;
} MainTrack;

typedef struct {
    int track_id;
    int net_id;
    int is_used;
} SignalAnalyzerTrack;

typedef struct {
	int r;
	int b;
	int g;
}RGB;

extern MainTrack mainTracks[32];

typedef struct {
    GPIO_TypeDef* port;
    uint32_t pin;
    const char* const (*xPins)[16];  // Pointer to an array of 16 strings
    const char* const (*yPins)[8];   // Pointer to an array of 8 strings
} MUX;

extern const MUX muxes[34];

const char* getPortName(GPIO_TypeDef* port);
const char* getPinName(uint32_t pin);

void printMUXDetails(MUX *mux);

void setConnection(int x, int y, MUX mux, uint8_t mode);

void routeBreadboard(int breadboardPin1, int breadboardPin2, int net_id, MUX *muxes, size_t muxCount, uint8_t mode, RGB rgb);

void routeSignalAnalyzer(int net_id, MUX *muxes, uint8_t mode);

void clear();

void processCommand(char *command);

#endif /* __MUX_H */
