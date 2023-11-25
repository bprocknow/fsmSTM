
#ifndef LED_DEF
#define LED_DEF

#include <stdbool.h>
#include "fsm.h"

typedef enum {
    BTN_PRESS_SIG = USER_SIG,
    TIMEOUT_SIG,
    BTN_TOGGLE,
} ReservedEvents;

extern Active *A_TimeApp;

void BSP_ToggleLed(void);

void BSP_SetLedEnable(bool en);

void BSP_BtnIrqHandler(void);

#endif