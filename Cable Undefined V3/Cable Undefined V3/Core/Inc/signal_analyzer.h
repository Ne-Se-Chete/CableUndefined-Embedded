#ifndef SIGNAL_ANALYZER_H
#define SIGNAL_ANALYZER_H

#include <stdint.h>

#define ADC_CHANNELS 8

typedef struct {
    uint8_t channel_enabled[8];  // 0 = disable, 1 = enable
} ADCChannelConfig;

extern ADCChannelConfig adcChannelConfig;

void SignalAnalyzer_Init(void);
void sendADCData(void);
uint32_t getTimestamp(void);

#endif // SIGNAL_ANALYZER_H
