
#ifndef APP_DEF
#define APP_DEF

#include <stdint.h>
#include "fsm.h"
#include "timer.h"

typedef struct TimeApp TimeApp;

struct TimeApp {
    // Inherit Active class
    Active super;

    Timer timer;
};

void TIMER_Ctr(TimeApp *me);

#endif
