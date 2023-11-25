
#ifndef TIMER_DEF
#define TIMER_DEF

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "fsm.h"

typedef struct Timer Timer;

extern Timer *A_Timer;

struct Timer {
    // Inherit Event
    Event super;

    Active *act;

    TIM_HandleTypeDef *htim;

    // Counter for the remaining timers to start, each sending signal cnt_sig
    uint32_t timeout_cnt;

    // Signal to send whenever cnt_sig is non-zero
    Event cnt_event;
};

void TIM_Ctr(Timer *tim, Active *act, TIM_HandleTypeDef *htim, Signal sig);

void TIM_StartTimer(Timer *tim);

void TIM_SetPeriod(Timer *tim, uint32_t period);

void TIM_SetTimeoutCntr(Timer *tim, Signal sig, uint32_t count_left);

void TIM_IrqHandler(void);

#endif