#ifndef PROCESS_RUNNER_H
#define PROCESS_RUNNER_H

typedef enum {
    READY, RUNNING, EXIT
} process_state_t;

typedef struct {
    char process_name[11];

    int entryTime;
    int serviceTime;
    int remainingTime;

    process_state_t status;
} pcb_t;



#endif