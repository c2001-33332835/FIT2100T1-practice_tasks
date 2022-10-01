#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "process_runner.h"
#include "list.h"
#include "logs.h"
#include "process_record.h"

#define TIMEWAIT 1

void pr_run_processes_fcfs(linked_node_t* processes, int output_fd){
    int finished = 0;
    unsigned process_count = list_length(processes);
    linked_node_t* pcbs = list_create_empty();
    int current_queue_position = 0;
    unsigned time_iteration = 0;
    int new_iteration = 1;
    pcb_t* current_process;
    while (finished < process_count){
        // log iteration
        if (new_iteration){
            log_iteration(time_iteration);
        }
        
        CURR_ROUND:

        // Get arriving records, conevrt and store into pcbs.
        linked_node_t* next = pr_next_processes(time_iteration, processes);
        for (int i = 0; i < list_length(next); i++){
            pcb_t* new_process = (pcb_t*) malloc(sizeof(pcb_t));
            process_record_t* new_process_record = (process_record_t*) list_get_item(next, i);
            strcpy(new_process->process_name, new_process_record->process_name);
            new_process->entry_time = new_process_record->arrive_time;
            new_process->remaining_time = new_process_record->service_time;
            new_process->service_time = new_process_record->service_time;
            new_process->deadline = new_process_record->deadline;
            new_process->start_time = 0;
            new_process->exit_time = 0;
            new_process->status = PS_READY;
            // put the new records in queue
            list_append_node(pcbs, new_process);

            log_pcb(new_process, LOG_ARRIVE, time_iteration, output_fd);
        }
        list_free_nodes(next);

        // check if any pending processes, if not, sleep this round and enter next round;
        if (current_queue_position >= list_length(pcbs)){
            goto NEXT_ROUND;
        }

        // Get currently running process
        current_process = (pcb_t*) list_get_item(pcbs, current_queue_position);

        // Detect if the process is newly executing in this ruond.
        if (current_process->remaining_time == current_process->service_time){
            log_pcb(current_process, LOG_START, time_iteration, output_fd);
            current_process->start_time = time_iteration;
            current_process->status = PS_RUNNING;
        }

        // Detect if the process has done execution in this round,
        // if yes, repeat the round and get a new process
        if (current_process->remaining_time == 0){
            finished ++; // increase the finished processes counter
            current_queue_position ++; // increase the index queue, to get next process
            current_process->status = PS_EXIT; // update process status
            current_process->exit_time = time_iteration; // update process status
            log_pcb(current_process, LOG_EXIT, time_iteration, output_fd);
            free(current_process);
            goto CURR_ROUND; // repeat the same round
        }

        NEXT_ROUND: 

        current_process->remaining_time --;
        sleep(TIMEWAIT);

        time_iteration++;
        new_iteration = 1;
    }

    list_free_nodes(pcbs);
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