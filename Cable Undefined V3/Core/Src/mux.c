#include "mux.h"

// Function to control the multiplexer and data pin
void setConnection(int x, int y, uint16_t cs, uint8_t mode) {
    if (x < 0 || x > 15 || y < 0 || y > 7) {
        HAL_GPIO_WritePin(DEBUG_GPIO, DEBUG_PIN_1, GPIO_PIN_SET);  // Error indication
        return;
    }

    HAL_Delay(5000);

    // Activate multiplexer
    HAL_GPIO_WritePin(CS_GPIO, cs, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(CS_GPIO, cs, GPIO_PIN_SET);

    HAL_GPIO_WritePin(DEBUG_GPIO, DEBUG_PIN_4, GPIO_PIN_SET);

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
    HAL_GPIO_WritePin(DAT_GPIO, DAT_PIN, GPIO_PIN_SET);

    // Strobe signal
    HAL_Delay(20);
    HAL_GPIO_WritePin(STB_GPIO, STB_PIN, GPIO_PIN_SET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(STB_GPIO, STB_PIN, GPIO_PIN_RESET);

    HAL_Delay(2000);

    HAL_GPIO_WritePin(DAT_GPIO, DAT_PIN, GPIO_PIN_RESET);

//    // Strobe signal again
//    HAL_GPIO_WritePin(STB_GPIO, STB_PIN, GPIO_PIN_SET);
//    HAL_Delay(20);
//    HAL_GPIO_WritePin(STB_GPIO, STB_PIN, GPIO_PIN_RESET);
//    HAL_Delay(20);
}

