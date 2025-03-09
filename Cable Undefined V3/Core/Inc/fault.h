#ifndef FAULT_H
#define FAULT_H

#include "main.h"
#include <stdio.h>

#define PROTECTION_GPIO GPIOE
#define PROTECTION_PIN LL_GPIO_PIN_11

#define FAULT_3V3_PIN    LL_GPIO_PIN_12
#define FAULT_3V3_PORT   GPIOE

#define FAULT_5V_PIN     LL_GPIO_PIN_13
#define FAULT_5V_PORT    GPIOE

// Global FAULT flags (set by interrupt, checked in while loop)
extern volatile uint8_t fault_3v3_triggered;
extern volatile uint8_t fault_5v_triggered;
extern volatile uint8_t fault_sent;

void FAULT_CheckAndReport(void);
void FAULT_5v_HandleInterrupt(void);
void FAULT_3v3_HandleInterrupt(void);
void FAULT_5v_HandleMsg(void);
void FAULT_3v3_HandleMsg(void);



#endif // FAULT_H
