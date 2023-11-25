
#include <stddef.h>
#include "fsm.h"

#define QUEUE_NEXT_IDX(cur_idx_) ((cur_idx_ + 1) % EVENT_QUEUE_SZ)
#define IS_QUEUE_FULL(me_) (QUEUE_NEXT_IDX(me_->event_queue_head) == me_->event_queue_tail)
#define IS_QUEUE_EMPTY(me_) (me_->event_queue_head == me_->event_queue_tail)

static const Event entry_sig = { ENTRY_SIG };
static const Event exit_sig = { EXIT_SIG };

/* --------------- Local Function Declarations ------------- */
static Event *ACT_GetEvent(Active * const me);

/* --------------- FSM Public Function Definitions ------------- */

void FSM_Ctr(Fsm * const me, StateHandler initState) {

    me->state = initState;
}

void FSM_Init(Fsm * const me, Event const * const e) {

    // Execute the init function of the application, which transitions to a state
    (*me->state)(me, e);

    // Execute the entry event of the first state
    (*me->state)(me, &entry_sig);
}

void FSM_Dispatch(Fsm * const me, Event const * const e) {

    StateHandler previous_state = me->state;
    State status;

    status = (*me->state)(me, e);

    if (status == TRAN_STATUS) {
        (*previous_state)(me, &exit_sig);
        (*me->state)(me, &entry_sig);
    }
}

/* --------------- Active Public Function Definitions ------------- */

void ACT_Ctr(Active * const me, StateHandler initState) {
    // Clear the Queue
    me->event_queue_head = 0;
    me->event_queue_tail = 0;
    FSM_Ctr(&me->super, initState);
}

void ACT_EventLoop(void* pdata) {

    Active* me = (Active *)pdata;

    FSM_Init(&me->super, (Event *)0);

    while(1) {
        Event *e = ACT_GetEvent(me);

        if (!e) {
            continue;
        }

        FSM_Dispatch(&me->super, e);
    }
}

void ACT_PushEvent(Active * const me, Event * const e) {
    if (!IS_QUEUE_FULL(me)) {
        me->event_queue[me->event_queue_head] = e;
        me->event_queue_head = QUEUE_NEXT_IDX(me->event_queue_head);
    }
}

/* --------------- Private Functions ----------- */

static Event *ACT_GetEvent(Active * const me) {

    if (!IS_QUEUE_EMPTY(me)) {
        Event* event = me->event_queue[me->event_queue_tail];
        me->event_queue_tail = QUEUE_NEXT_IDX(me->event_queue_tail);
        return event;
    }
    return NULL;
}
