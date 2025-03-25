#ifndef SIGNAL_ANALYZER_H
#define SIGNAL_ANALYZER_H

#include "main.h"

// Function Prototypes
void SignalAnalyzer_Init(void);
void sendADCData(void);
void readADC(void);
uint32_t getTimestamp(void);

#endif // SIGNAL_ANALYZER_H
