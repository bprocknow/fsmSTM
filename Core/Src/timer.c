
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "timer.h"

void TIM_Ctr(Timer *tim, Active *act, TIM_HandleTypeDef *htim, Signal sig) {

    tim->super.sig = sig;
    tim->act = act;
    tim->htim = htim;
    tim->timeout_cnt = 0;

  __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
}

void TIM_StartTimer(Timer *tim) {
    HAL_TIM_Base_Start_IT(tim->htim);
}

void TIM_SetPeriod(Timer *tim, uint32_t period) {
    __HAL_TIM_SET_AUTORELOAD(tim->htim, period);
}

void TIM_SetTimeoutCntr(Timer *tim, Signal sig, uint32_t count_left) {
    tim->cnt_event.sig = sig;
    tim->timeout_cnt = count_left;
}

void TIM_IrqHandler() {

    if (A_Timer->timeout_cnt == 0) {
        ACT_PushEvent(A_Timer->act, &A_Timer->super);
    } else {
        ACT_PushEvent(A_Timer->act, &A_Timer->cnt_event);
        A_Timer->timeout_cnt--;
        HAL_TIM_Base_Start_IT(A_Timer->htim);
    }
}
