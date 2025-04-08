#ifndef SIGNAL_ANALYZER_H
#define SIGNAL_ANALYZER_H

#include <stdint.h>

#define ADC_CHANNELS 8

typedef struct {
    uint8_t channel_enabled[8];  // 0 = disable, 1 = enable
} ADCChannelConfig;

typedef enum {
    SIGNAL_MODE_ADC,
    SIGNAL_MODE_DIGITAL
} SignalMode_t;

extern volatile SignalMode_t signalMode;  // <- only declared here, not defined

extern ADCChannelConfig adcChannelConfig;

void OscilloscopeInit(void);
void OscilloscopeDeinit(void);

void sendADCData(void);
void sendDigitalData(void);


void sendSignalData(void); // Choses from ADC or DIgital

uint32_t getTimestamp(void);

#endif // SIGNAL_ANALYZER_H
