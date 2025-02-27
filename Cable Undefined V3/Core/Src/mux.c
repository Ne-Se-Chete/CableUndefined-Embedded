#include "mux.h"
#include "mux_config.h"

#include <string.h>
#include <stdio.h>

// Define CS configurations for all 34 multiplexers
const MUX muxes[34] = {
    {GPIOC, LL_GPIO_PIN_0, &xPins1, &yPins1},  // CS_1 (PC0)
    {GPIOC, LL_GPIO_PIN_1, &xPins2, &yPins2},  // CS_2 (PC1)
    {GPIOC, LL_GPIO_PIN_2, &xPins3, &yPins3},  // CS_3 (PC2)
    {GPIOC, LL_GPIO_PIN_3, &xPins4, &yPins4},  // CS_4 (PC3)
    {GPIOC, LL_GPIO_PIN_4, &xPins5, &yPins5},  // CS_5 (PC4)
    {GPIOC, LL_GPIO_PIN_5, &xPins6, &yPins6},  // CS_6 (PC5)
    {GPIOC, LL_GPIO_PIN_6, &xPins7, &yPins7},  // CS_7 (PC6)
    {GPIOC, LL_GPIO_PIN_7, &xPins8, &yPins8},  // CS_8 (PC7)
    {GPIOC, LL_GPIO_PIN_8, &xPins9, &yPins9},  // CS_9 (PC8)
    {GPIOC, LL_GPIO_PIN_9, &xPins10, &yPins10},  // CS_10 (PC9)
    {GPIOC, LL_GPIO_PIN_11, &xPins11, &yPins11}, // CS_11 (PC11)
    {GPIOC, LL_GPIO_PIN_13, &xPins12, &yPins12}, // CS_12 (PC13)
    {GPIOD, LL_GPIO_PIN_0, &xPins13, &yPins13},  // CS_13 (PD0)
    {GPIOD, LL_GPIO_PIN_1, &xPins14, &yPins14},  // CS_14 (PD1)
    {GPIOD, LL_GPIO_PIN_3, &xPins15, &yPins15},  // CS_15 (PD3)
    {GPIOD, LL_GPIO_PIN_4, &xPins16, &yPins16},  // CS_16 (PD4)
    {GPIOD, LL_GPIO_PIN_7, &xPins17, &yPins17},  // CS_17 (PD7)
    {GPIOD, LL_GPIO_PIN_8, &xPins18, &yPins18},  // CS_18 (PD8)
    {GPIOD, LL_GPIO_PIN_9, &xPins19, &yPins19},  // CS_19 (PD9)
    {GPIOD, LL_GPIO_PIN_10, &xPins20, &yPins20}, // CS_20 (PD10)
    {GPIOD, LL_GPIO_PIN_11, &xPins21, &yPins21}, // CS_21 (PD11)
    {GPIOD, LL_GPIO_PIN_12, &xPins22, &yPins22}, // CS_22 (PD12)
    {GPIOD, LL_GPIO_PIN_13, &xPins23, &yPins23}, // CS_23 (PD13)
    {GPIOD, LL_GPIO_PIN_15, &xPins24, &yPins24}, // CS_24 (PD15)
    {GPIOE, LL_GPIO_PIN_0, &xPins25, &yPins25},  // CS_25 (PE0)
    {GPIOE, LL_GPIO_PIN_1, &xPins26, &yPins26},  // CS_26 (PE1)
    {GPIOE, LL_GPIO_PIN_2, &xPins27, &yPins27},  // CS_27 (PE2)
    {GPIOE, LL_GPIO_PIN_3, &xPins28, &yPins28},  // CS_28 (PE3)
    {GPIOE, LL_GPIO_PIN_4, &xPins29, &yPins29},  // CS_29 (PE4)
    {GPIOE, LL_GPIO_PIN_5, &xPins30, &yPins30},  // CS_30 (PE5)
    {GPIOE, LL_GPIO_PIN_6, &xPins31, &yPins31},  // CS_31 (PE6)
    {GPIOE, LL_GPIO_PIN_7, &xPins32, &yPins32},  // CS_32 (PE7)
    {GPIOE, LL_GPIO_PIN_8, &xPins33, &yPins33},  // CS_33 (PE8)
    {GPIOE, LL_GPIO_PIN_9, &xPins34, &yPins34}   // CS_34 (PE9)
};

const char* getPortName(GPIO_TypeDef* port) {
    if (port == GPIOA) return "GPIOA";
    if (port == GPIOB) return "GPIOB";
    if (port == GPIOC) return "GPIOC";
    if (port == GPIOD) return "GPIOD";
    if (port == GPIOE) return "GPIOE";
    return "UNKNOWN";
}

MainTrack mainTracks[32] = {0};

// Set connection function with dynamic CS handling
void setConnection(int x, int y, MUX mux, uint8_t mode) {
    if (x < 0 || x > 15 || y < 0 || y > 7) {
        return;
    }


    // Activate multiplexer using the struct for CS
    LL_GPIO_ResetOutputPin(mux.port, mux.pin);
    LL_mDelay(20);
    LL_GPIO_SetOutputPin(mux.port, mux.pin);

    // Set X address
    // Set X address
    (x & 0x01) ? LL_GPIO_SetOutputPin(AX_GPIO, AX0_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX0_PIN);
    (x & 0x02) ? LL_GPIO_SetOutputPin(AX_GPIO, AX1_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX1_PIN);
    (x & 0x04) ? LL_GPIO_SetOutputPin(AX_GPIO, AX2_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX2_PIN);
    (x & 0x08) ? LL_GPIO_SetOutputPin(AX_GPIO, AX3_PIN) : LL_GPIO_ResetOutputPin(AX_GPIO, AX3_PIN);

    // Set Y address
    (y & 0x01) ? LL_GPIO_SetOutputPin(AY_GPIO, AY0_PIN) : LL_GPIO_ResetOutputPin(AY_GPIO, AY0_PIN);
    (y & 0x02) ? LL_GPIO_SetOutputPin(AY_GPIO, AY1_PIN) : LL_GPIO_ResetOutputPin(AY_GPIO, AY1_PIN);
    (y & 0x04) ? LL_GPIO_SetOutputPin(AY_GPIO, AY2_PIN) : LL_GPIO_ResetOutputPin(AY_GPIO, AY2_PIN);


    // Set data pin
    mode ? LL_GPIO_SetOutputPin(DAT_GPIO, DAT_PIN) : LL_GPIO_ResetOutputPin(DAT_GPIO, DAT_PIN);

    // Strobe signal
    LL_mDelay(20);
	LL_GPIO_SetOutputPin(STB_GPIO, STB_PIN);
	LL_mDelay(20);
	LL_GPIO_ResetOutputPin(STB_GPIO, STB_PIN);

	LL_mDelay(200);

	LL_GPIO_ResetOutputPin(DAT_GPIO, DAT_PIN);
}

void route(int breadboardPin1, int breadboardPin2, int net_id, MUX *muxes, size_t muxCount, uint8_t mode, RGB rgb) {

    char pin1Name[6], pin2Name[6];
    snprintf(pin1Name, sizeof(pin1Name), "B_%d", breadboardPin1);
    snprintf(pin2Name, sizeof(pin2Name), "B_%d", breadboardPin2);


    printf("Routing %s to %s with net ID: %d\n", pin1Name, pin2Name, net_id);
    fflush(stdout);

    int xIndex1 = -1, yIndex1 = -1;
    int xIndex2 = -1, yIndex2 = -1;
    MUX *mux1 = NULL;
    MUX *mux2 = NULL;
    MainTrack *selectedTrack = NULL;
    int muxIndex = -1;

    // Find an available or existing main track for the given net ID
    for (int x = 0; x < 32; ++x) {
        if (mainTracks[x].net_id == net_id) {
            selectedTrack = &mainTracks[x];  // Reuse existing track if net ID matches
            break;
        } else if (!mainTracks[x].is_used) {
            selectedTrack = &mainTracks[x];  // Assign new track if available
            selectedTrack->is_used = 1;
            selectedTrack->net_id = net_id;
            selectedTrack->track_id = x; // Assign track ID here
            printf("Selected Track id: %d\n", x);
            break;
        }
    }

    if (!selectedTrack) {
        printf("Error: No available main track for net ID %d\n", net_id);
        return;
    }

    // Update connection counter based on mode
	if (mode == 1) {
		selectedTrack->current_connections++; // Increment current connections
		printf("Main Track %d incremented. Current connections: %d\n", selectedTrack->track_id, selectedTrack->current_connections);
	} else if (mode == 0) {
		selectedTrack->current_connections--; // Decrement current connections
		if (selectedTrack->current_connections <= 0) { // If counter is 0, free track
			printf("Main Track %d is now free\n", selectedTrack->track_id);
			selectedTrack->is_used = 0;
			selectedTrack->net_id = -1;
			selectedTrack->current_connections = 0;
		}else{
			printf("Main Track %d decremented, but still occupied. Current connections: %d\n", selectedTrack->track_id, selectedTrack->current_connections);

		}
	}

    muxIndex = (selectedTrack->track_id < 16) ? 0 : 1;

        for (size_t i = muxIndex; i < muxCount; i += 2) {
            for (int y = 0; y < 8; ++y) {
                if (strcmp((*muxes[i].yPins)[y], pin1Name) == 0 && mux1 == NULL) {
                    xIndex1 = selectedTrack->track_id % 16;
                    yIndex1 = y;
                    mux1 = &muxes[i];
                }
                if (strcmp((*muxes[i].yPins)[y], pin2Name) == 0 && mux2 == NULL) {
                    xIndex2 = selectedTrack->track_id % 16;
                    yIndex2 = y;
                    mux2 = &muxes[i];
                }
            }
            if (mux1 && mux2) break;
        }

        if (mux1 && mux2) {
        	printf("%s (X:%d, Y:%d) at MUX[%d] CS: %s, Pin: %d, Main Track: %d\n"
        			"%s (X:%d, Y:%d) at MUX[%d] CS: %s, Pin: %d, Main Track: %d\n\n",
        	                mode ? "Connecting" : "Disconnecting",
        	                xIndex1, yIndex1, mux1 - muxes + 1, getPortName(mux1->port), __builtin_ctz(mux1->pin), selectedTrack->track_id,
							mode ? "Connecting" : "Disconnecting",
        	                xIndex2, yIndex2, mux2 - muxes + 1, getPortName(mux2->port), __builtin_ctz(mux2->pin), selectedTrack->track_id);
        	        fflush(stdout);
			fflush(stdout);


            setConnection(xIndex1, yIndex1, *mux1, mode);
            setConnection(xIndex2, yIndex2, *mux2, mode);

// 			  Activate leds

        } else {
            printf("Error: Unable to route pins - No available MUX found!\n");
            fflush(stdout);
        }
    }

void processCommand(char *command) {
    int pin1, pin2, net_id, mode, r, g, b;

    if (strncmp(command, "ROUTE", 5) == 0) {  // Check if it's a "ROUTE" command
        int parsed = sscanf(command, "ROUTE %d %d %d %d %d %d %d",
                            &pin1, &pin2, &net_id, &mode, &r, &g, &b);
        if (parsed == 7) {  // Ensure all arguments were parsed
            RGB color = {r, g, b};
            printf("Calling route with: Pin1=%d, Pin2=%d, NetID=%d, Mode=%d, RGB(%d,%d,%d)\n",
                   pin1, pin2, net_id, mode, r, g, b);
            fflush(stdout);

            route(pin1, pin2, net_id, muxes, sizeof(muxes) / sizeof(muxes[0]), mode, color);
        } else {
            printf("Error: Invalid ROUTE command format!\n");
            fflush(stdout);
        }
    }
}
