#ifndef PROCESS_RUNNER_H
#define PROCESS_RUNNER_H
#include "list.h"

typedef enum {
    READY, RUNNING, EXIT
} process_state_t;

typedef struct {
    char process_name[11];

    int entry_time;
    int service_time;
    int remaining_time;

    process_state_t status;
} pcb_t;

void run_processes(linked_node_t* processes);
linked_node_t* next_processes(int t, linked_node_t* processes);

#endif