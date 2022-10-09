#ifndef PROCESS_RUNNER_H
#define PROCESS_RUNNER_H
#include <stdio.h>
#include "list.h"
#include "process_record.h"

/* This is the library that simulates the execution process and scheduling algorithms.
 * This library contains 3 different algorithms:
 *  - the First Come First Serve (FCFS) algorithm
 *  - the Round Robin (rr) algorithm
 *  - the Urgent First (uf) algorithm (designed and implemented by myself, Xuanao Zhao 33332835)
 * as well as some utilities function useful in the simulation process.
 *
 * This is the header file for interface and intellisense 
 * The implementation of this file is in process_runner.c
 */

typedef enum {
    PS_READY, PS_RUNNING, PS_EXIT, PS_PAUSED
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
void pr_run_processes_rr(linked_node_t* processes, int time_quant ,int output_fd);
void pr_run_processes_uf(linked_node_t* processes, int output_fd);
pcb_t* pr_record_to_pcb(process_record_t* record);
linked_node_t* pr_next_processes(int t, linked_node_t* processes);
void pr_print_pcb(pcb_t* pcb);

#endif