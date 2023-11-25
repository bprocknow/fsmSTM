
#include "stm32f1xx_hal.h"
#include "bsp.h"
#include "fsm.h"

static Event btn_press = { BTN_PRESS_SIG };

void BSP_ToggleLed() {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
}

void BSP_SetLedEnable(bool en) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, en ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BSP_BtnIrqHandler(void) {
    ACT_PushEvent(A_TimeApp, &btn_press);
}