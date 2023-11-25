
#ifndef FSM_DEF
#define FSM_DEF

#include <stdint.h>

#define EVENT_QUEUE_SZ 10

typedef enum { 
    ENTRY_SIG, 
    EXIT_SIG,
    USER_SIG,
} Signal;

typedef struct Event Event;

/* -------------- FSM Types ------------------- */

typedef struct Fsm Fsm;

typedef enum { TRAN_STATUS, INIT_STATUS, IGNORED_STATUS, HANDLED_STATUS } State;

typedef State (*StateHandler)(Fsm * const me, Event const * const e);

#define TRAN(target_) (((Fsm *)me)->state = (StateHandler)(target_), TRAN_STATUS)

struct Fsm {
    StateHandler state;
};

/* -------------- Active Types ------------------- */

typedef struct Active Active;

struct Event {
    uint16_t sig;
};

struct Active {
    // Inherit FSM 
    Fsm super;

    int event_queue_head, event_queue_tail;
    Event *event_queue[EVENT_QUEUE_SZ];
};

/* --------------- FSM Public Functions ------------ */

/**
 * @def FSM_Ctr
 * @brief Constructor for the Finite State Machine
 * @param[in] me:  Private data structure for FSM data
 * @param[in] initState: Initial state handler function
*/
void FSM_Ctr(Fsm * const me, StateHandler initState);

/**
 * @def FSM_Init
 * @brief Initialization function for the fsm
 * @param[in] me: Private data structure for FSM data
 * @param[in] e: Initial event to run on the initial state
*/
void FSM_Init(Fsm * const me, Event const * const e);

/**
 * @def FSM_Dispatch
 * @brief Executes specified events on the current state handler function
 * @param[in] me: Private data structure for FSM data
 * @param[in] e: Event to run on the current state
*/
void FSM_Dispatch(Fsm * const me, Event const * const e);

/* ----------------- Active Public Functions ---------- */

void ACT_Ctr(Active * const me, StateHandler initState);

void ACT_EventLoop(void* pdata);

void ACT_PushEvent(Active * const me, Event * const e);


#endif
