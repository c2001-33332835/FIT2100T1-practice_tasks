#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "process_runner.h"
#include "list.h"
#include "logs.h"
#include "process_record.h"

#define TIMEWAIT 0

void pr_run_processes_fcfs(linked_node_t* processes, int output_fd){
    int total_process_count = (int) list_length(processes);

    // create a list to store pcbs currently in system.
    linked_node_t* pcbs = list_create_empty(); // execution queue
    int time = 0; // the time iteration (t value).
    int done = 0; // number of processes exitted.

    while (1){
        // log this iteration
        log_iteration(time);

        // check if there is new processes entering in thie new iteration
        linked_node_t* next = pr_next_processes(time, processes);
        for (int i = 0; i < list_length(next); i++){
            process_record_t* record = (process_record_t*) list_get_item(next, i);
            pcb_t* process = pr_record_to_pcb(record);
            process->entry_time = time;
            list_append_node(pcbs, process);
            log_pcb(process, LOG_ARRIVE, time, output_fd); // log this arrival event
        }
        list_free_nodes(next);

        // check if any process in queue has missed their deadline now
        for (int i = 0; i < list_length(pcbs); i++){
            pcb_t* process = (pcb_t*) list_get_item(pcbs, i);
            int elapsed = time - process->entry_time;
            if (elapsed == process->deadline && process->remaining_time != 0){
                log_pcb(process, LOG_DEADLINEMISS, time, output_fd);
            }
        }

        CURRENT_ITERATION:

        // if queue is empty, continue to next iteration
        if (list_length(pcbs) == 0){
            goto NEXT_ITERATION;
        }

        // first element in queue is the current process
        pcb_t* current = (pcb_t*) list_get_item(pcbs, 0);

        // check if the current process is starting
        if (current->status == PS_READY){
            current->status = PS_RUNNING;
            current->start_time = time;
            log_pcb(current, LOG_START, time, output_fd); // log this execution event
        }

        // check if the process is exitting in this iteration
        if (current->status == PS_RUNNING && current->remaining_time == 0){
            current->exit_time = time;
            current->status = PS_EXIT;
            log_pcb(current, LOG_EXIT, time, output_fd); // log this exit event
            done ++;

            // remove this pcb from list
            pcbs = list_remove_item(pcbs, 0);

            // check if all processes has executed
            if (done == total_process_count){
                break;
            }

            // redo this iteration to get next starting event
            goto CURRENT_ITERATION;
        }

        NEXT_ITERATION:

        // increment time iteration by 1
        sleep(TIMEWAIT);
        current->remaining_time --;
        time++;
    }

    list_free(pcbs);
}

void pr_run_processes_rr(linked_node_t* processes, int time_quant ,int output_fd){

}

pcb_t* pr_record_to_pcb(process_record_t* record){
    pcb_t* pcb = (pcb_t*) malloc(sizeof(pcb_t));
    strcpy(pcb->process_name, record->process_name);
    pcb->entry_time = 0;
    pcb->service_time = record->service_time;
    pcb->remaining_time = record->service_time;
    pcb->deadline = record->deadline;
    pcb->start_time = 0;
    pcb->exit_time = 0;
    pcb->status = PS_READY;
    return pcb;
}

linked_node_t* pr_next_processes(int t, linked_node_t* processes){
    linked_node_t* next = list_create_empty();
    for (int i = 0; i < list_length(processes); i++){
        process_record_t* record = (process_record_t*) list_get_item(processes, i);
        if (record->arrive_time == t){
            list_append_node(next, record);
        }
    }
    return next;
}

void pr_print_pcb(pcb_t* pcb){
    char* process_name = pcb->process_name;
    int entry_time = pcb->entry_time;
    int service_time = pcb->service_time;
    int remaining_time = pcb->remaining_time;

    printf("<pcb_t n='%s' e=%d s=%d r=%d>\n", process_name, entry_time, service_time, remaining_time);
};