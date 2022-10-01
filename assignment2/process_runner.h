#ifndef PROCESS_RUNNER_H
#define PROCESS_RUNNER_H
#include <stdio.h>
#include "list.h"

typedef enum {
    PS_READY, PS_RUNNING, PS_EXIT
} process_state_t;

typedef struct {
    char process_name[11];

    int entry_time;
    int service_time;
    int remaining_time;
    int deadline;
    int start_time;
    int exit_time;
    process_state_t status;
} pcb_t;

void pr_run_processes_fcfs(linked_node_t* processes, int output_fd);
linked_node_t* pr_next_processes(int t, linked_node_t* processes);
void pr_print_pcb(pcb_t* pcb);

#endif