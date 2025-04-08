#include "fault.h"
#include "serial.h"
#include "main.h"

// Global variables to track FAULT events
volatile uint8_t fault_3v3_triggered;
volatile uint8_t fault_5v_triggered;

volatile uint8_t fault_sent;

static char msg[20];

/**
 * @brief Check which FAULT occurred and send the appropriate message
 */
void FAULT_CheckAndReport(void)
{
    // Only process if an interrupt was triggered and message has not been sent
    if (!fault_sent) {
        uint8_t fault_msg[2] = {0xFF, 0x00};  // 0xFF = start byte, 0x00 = code

		/*
		[0xFF] [0x01] for 3v3
		[0xFF] [0x02] for 5V
		[0xFF] [0x03] for both
		 */

        if (fault_3v3_triggered && fault_5v_triggered) {
            fault_msg[1] = 0x03; // Both
        } else if (fault_3v3_triggered) {
            fault_msg[1] = 0x01; // 3v3 only
        } else if (fault_5v_triggered) {
            fault_msg[1] = 0x02; // 5V only
        } else {
            return; // No fault
        }

        // Send hex-formatted message via UART1 and UART3
        if (usingESP)
        	sendRawUART(USART1, fault_msg, 2);
        else if (usingCP2102)
        	sendRawUART(USART3, fault_msg, 2);
    }

    fault_sent = 1;
}


/**
 * @brief Handles the 5V FAULT interrupt, only sets flags
 */
void FAULT_5v_HandleInterrupt(void) {
    // Set trigger flag when interrupt occurs
	fault_5v_triggered = 1;
    fault_sent = 0;  // Reset message sent flag to allow new transmission
}

/**
 * @brief Handles the 5V FAULT interrupt, only sets flags
 */
void FAULT_3v3_HandleInterrupt(void) {
    // Set trigger flag when interrupt occurs
	fault_3v3_triggered = 1;
    fault_sent = 0;  // Reset message sent flag to allow new transmission
}

void FAULT_5v_HandleMsg(void){
	printf("Reseting the protection gpio, 5V");
	fflush(stdout);

	fault_5v_triggered = 0;
    fault_sent = 0;

    LL_GPIO_ResetOutputPin(PROTECTION_GPIO, PROTECTION_PIN);
    LL_mDelay(20);
    LL_GPIO_SetOutputPin(PROTECTION_GPIO, PROTECTION_PIN);
}

void FAULT_3v3_HandleMsg(void){
	printf("reseting the protection gpio, 3V3");
	fflush(stdout);

	fault_3v3_triggered = 0;
    fault_sent = 0;

    LL_GPIO_ResetOutputPin(PROTECTION_GPIO, PROTECTION_PIN);
    LL_mDelay(20);
    LL_GPIO_SetOutputPin(PROTECTION_GPIO, PROTECTION_PIN);
}


