#ifndef PROCESS_STRUCT_H
#define PROCESS_STRUCT_H

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