
#include "stm32f1xx_hal.h"
#include "app.h"
#include "bsp.h"
#include "timer.h"

#define FLASH_PERIOD 500
#define BOOM_PERIOD 50

static State TIMER_Initial(TimeApp * const me, Event const * const e);
static State TIMER_WaitForButton(TimeApp * const me, Event const * const e);
static State TIMER_Flash(TimeApp * const me, Event const * const e);
static State TIMER_Boom(TimeApp * const me, Event const * const e);

void TIMER_Ctr(TimeApp *me) {
    ACT_Ctr(&me->super, &TIMER_Initial);
}

static State TIMER_Initial(TimeApp * const me, Event const * const e) {
    return TRAN(&TIMER_WaitForButton);
}

static State TIMER_WaitForButton(TimeApp * const me, Event const * const e) {

    State status;

    switch (e->sig) {
        case (ENTRY_SIG): {
            BSP_SetLedEnable(true);
            status = HANDLED_STATUS;
            break;
        }
        case (BTN_PRESS_SIG): {
            status = TRAN(TIMER_Flash);
            break;
        }
        case (EXIT_SIG): {
            BSP_SetLedEnable(false);
            status = HANDLED_STATUS;
            break;
        }
        default: {
            status = IGNORED_STATUS;
            break;
        }
    }

    return status;
}

static State TIMER_Flash(TimeApp * const me, Event const * const e) {
	State status;

    switch (e->sig) {
        case (ENTRY_SIG): {
            TIM_SetPeriod(&me->timer, FLASH_PERIOD);
            TIM_SetTimeoutCntr(&me->timer, BTN_TOGGLE, 5);
            TIM_StartTimer(&me->timer);
            status = HANDLED_STATUS;
            break;
        }
        case (EXIT_SIG): {
            BSP_SetLedEnable(false);
            status = HANDLED_STATUS;
            break;
        }
        case (TIMEOUT_SIG): {
            BSP_SetLedEnable(false);
            status = TRAN(TIMER_Boom);
            break;
        }
        case (BTN_TOGGLE): {
            BSP_ToggleLed();
            status = HANDLED_STATUS;
            break;
        }
        default:  {
            status = IGNORED_STATUS;
            break;
        }
    }

	return status;
}

static State TIMER_Boom(TimeApp * const me, Event const * const e) {
	State status;

    switch (e->sig) {
        case (ENTRY_SIG): {
            TIM_SetPeriod(&me->timer, BOOM_PERIOD);
            TIM_SetTimeoutCntr(&me->timer, BTN_TOGGLE, 10);
            TIM_StartTimer(&me->timer);
            status = HANDLED_STATUS;
            break;
        }
        case (EXIT_SIG): {
            status = HANDLED_STATUS;
            break;
        }
        case (TIMEOUT_SIG): {
            status = TRAN(TIMER_WaitForButton);
            break;
        }
        case (BTN_TOGGLE): {
            BSP_ToggleLed();
            status = HANDLED_STATUS;
            break;
        }
        default: {
            status = IGNORED_STATUS;
            break;
        }
    }

	return status;
}
