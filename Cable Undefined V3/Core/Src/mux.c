#include "mux.h"
#include "mux_config.h"
#include <string.h>
#include <stdio.h>

// Define CS configurations for all 34 multiplexers
const MUX muxes[34] = {
    {GPIOC, GPIO_PIN_0, &xPins1, &yPins1},  // CS_1 (PC0)
    {GPIOC, GPIO_PIN_1, &xPins2, &yPins2},  // CS_2 (PC1)
    {GPIOC, GPIO_PIN_2, &xPins3, &yPins3},  // CS_3 (PC2)
    {GPIOC, GPIO_PIN_3, &xPins4, &yPins4},  // CS_4 (PC3)
    {GPIOC, GPIO_PIN_4, &xPins5, &yPins5},  // CS_5 (PC4)
    {GPIOC, GPIO_PIN_5, &xPins6, &yPins6},  // CS_6 (PC5)
    {GPIOC, GPIO_PIN_6, &xPins7, &yPins7},  // CS_7 (PC6)
    {GPIOC, GPIO_PIN_7, &xPins8, &yPins8},  // CS_8 (PC7)
    {GPIOC, GPIO_PIN_8, &xPins9, &yPins9},  // CS_9 (PC8)
    {GPIOC, GPIO_PIN_9, &xPins10, &yPins10},  // CS_10 (PC9)
    {GPIOC, GPIO_PIN_11, &xPins11, &yPins11}, // CS_11 (PC11)
    {GPIOC, GPIO_PIN_13, &xPins12, &yPins12}, // CS_12 (PC13)
    {GPIOD, GPIO_PIN_0, &xPins13, &yPins13},  // CS_13 (PD0)
    {GPIOD, GPIO_PIN_1, &xPins14, &yPins14},  // CS_14 (PD1)
    {GPIOD, GPIO_PIN_3, &xPins15, &yPins15},  // CS_15 (PD3)
    {GPIOD, GPIO_PIN_4, &xPins16, &yPins16},  // CS_16 (PD4)
    {GPIOD, GPIO_PIN_7, &xPins17, &yPins17},  // CS_17 (PD7)
    {GPIOD, GPIO_PIN_8, &xPins18, &yPins18},  // CS_18 (PD8)
    {GPIOD, GPIO_PIN_9, &xPins19, &yPins19},  // CS_19 (PD9)
    {GPIOD, GPIO_PIN_10, &xPins20, &yPins20}, // CS_20 (PD10)
    {GPIOD, GPIO_PIN_11, &xPins21, &yPins21}, // CS_21 (PD11)
    {GPIOD, GPIO_PIN_12, &xPins22, &yPins22}, // CS_22 (PD12)
    {GPIOD, GPIO_PIN_13, &xPins23, &yPins23}, // CS_23 (PD13)
    {GPIOD, GPIO_PIN_15, &xPins24, &yPins24}, // CS_24 (PD15)
    {GPIOE, GPIO_PIN_0, &xPins25, &yPins25},  // CS_25 (PE0)
    {GPIOE, GPIO_PIN_1, &xPins26, &yPins26},  // CS_26 (PE1)
    {GPIOE, GPIO_PIN_2, &xPins27, &yPins27},  // CS_27 (PE2)
    {GPIOE, GPIO_PIN_3, &xPins28, &yPins28},  // CS_28 (PE3)
    {GPIOE, GPIO_PIN_4, &xPins29, &yPins29},  // CS_29 (PE4)
    {GPIOE, GPIO_PIN_5, &xPins30, &yPins30},  // CS_30 (PE5)
    {GPIOE, GPIO_PIN_6, &xPins31, &yPins31},  // CS_31 (PE6)
    {GPIOE, GPIO_PIN_7, &xPins32, &yPins32},  // CS_32 (PE7)
    {GPIOE, GPIO_PIN_8, &xPins33, &yPins33},  // CS_33 (PE8)
    {GPIOE, GPIO_PIN_9, &xPins34, &yPins34}   // CS_34 (PE9)
};

const char* getPortName(GPIO_TypeDef* port) {
    if (port == GPIOA) return "GPIOA";
    if (port == GPIOB) return "GPIOB";
    if (port == GPIOC) return "GPIOC";
    if (port == GPIOD) return "GPIOD";
    if (port == GPIOE) return "GPIOE";
    return "UNKNOWN";
}



// Set connection function with dynamic CS handling
void setConnection(int x, int y, MUX mux, uint8_t mode) {
    if (x < 0 || x > 15 || y < 0 || y > 7) {
        return;
    }

//    HAL_Delay(5000);

    // Activate multiplexer using the struct for CS
    HAL_GPIO_WritePin(mux.port, mux.pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(mux.port, mux.pin, GPIO_PIN_SET);

    // Set X address
    HAL_GPIO_WritePin(AX_GPIO, AX0_PIN, (x & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AX_GPIO, AX1_PIN, (x & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AX_GPIO, AX2_PIN, (x & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AX_GPIO, AX3_PIN, (x & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Set Y address
    HAL_GPIO_WritePin(AY_GPIO, AY0_PIN, (y & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AY_GPIO, AY1_PIN, (y & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AY_GPIO, AY2_PIN, (y & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Set data pin
    HAL_GPIO_WritePin(DAT_GPIO, DAT_PIN, mode ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Strobe signal
    HAL_Delay(20);
    HAL_GPIO_WritePin(STB_GPIO, STB_PIN, GPIO_PIN_SET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(STB_GPIO, STB_PIN, GPIO_PIN_RESET);

    HAL_Delay(200);

    HAL_GPIO_WritePin(DAT_GPIO, DAT_PIN, GPIO_PIN_RESET);
}

MainTrack mainTracks[32] = {0};

void route(int breadboardPin1, int breadboardPin2, int net_id, MUX *muxes, size_t muxCount, uint8_t mode) {

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


//            setConnection(xIndex1, yIndex1, *mux1, mode);
//            setConnection(xIndex2, yIndex2, *mux2, mode);

        } else {
            printf("Error: Unable to route pins - No available MUX found!\n");
            fflush(stdout);
        }
    }
