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
    /* 
     * This function runs a list of processes stored as process_record_t
     * using the first come first serve alrgorithm.
     * The output is printed to stdout and written to the output file.
     * ARGUMENTS:
     *  - processes: any node from a linked list (list.h),
     *               in the type of process_record_t (process_record.h)
     *               The list of processes to be executed.
     *  - output_fd: an integer, the file descriptor of output file.
     */
    
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
    /* 
     * This function runs a list of processes stored as process_record_t
     * using the round robin alrgorithm.
     * The output is printed to stdout and written to the output file.
     * ARGUMENTS:
     *  - processes: any node from a linked list (list.h),
     *               in the type of process_record_t (process_record.h)
     *               The list of processes to be executed.
     *  - time_quant: an integer, the time quantum for round robin algorithm.
     *  - output_fd: an integer, the file descriptor of output file.
     */

    int total_process_count = (int) list_length(processes);

    // create a list to store pcbs currently in system.
    linked_node_t* pcbs = list_create_empty(); // execution queue
    int time = 0; // the time iteration (t value).
    int done = 0; // number of processes exitted.
    int quantprog = 0; // the current progress in a time quantum

    while (1){
        sleep(TIMEWAIT);
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

        CURRENT_ROUND:

        // if no process arrived yet, skip iteration.
        if (list_length(pcbs) == 0){
            quantprog = 0;
            goto NEXT_ROUND;
        }

        // get first pcb in queue
        pcb_t* current = (pcb_t*) list_get_item(pcbs, 0);

        // check if quantum has expired
        if (quantprog >= time_quant && current->remaining_time != 0){
            quantprog = 0;
            // append current task to the end, and remove the current task
            pcb_t* queue_process = (pcb_t*) list_get_item(pcbs, 0);
            queue_process->status = PS_PAUSED;
            log_pcb(queue_process, LOG_SUSPENSION, time, output_fd); // log pausing event
            list_append_node(pcbs, queue_process);
            pcbs = list_remove_node(pcbs, 0);
            goto CURRENT_ROUND; // redo this round if quantum expired
        }

        // if first time running this process, start the process
        if (current->status == PS_READY){
            quantprog = 0;
            current->status = PS_RUNNING;
            current->start_time = time;
            log_pcb(current, LOG_START, time, output_fd);
        }

        // if process is paused, resume the process
        if (current->status == PS_PAUSED){
            quantprog = 0;
            current->status = PS_RUNNING;
            log_pcb(current, LOG_RESUME, time, output_fd);
        }

        // if the process ends in this round
        if (current->status == PS_RUNNING && current->remaining_time == 0){
            done ++;
            current->exit_time = time;
            log_pcb(current, LOG_EXIT, time, output_fd);
            pcbs = list_remove_item(pcbs, 0); // remove and free pcb from list
            quantprog = 0;
            goto CURRENT_ROUND; // redo this round to start executing the next process
        }

        // run the process if not finished, increase time quantum progress
        if (current->status == PS_RUNNING){
            current->remaining_time --;
            goto NEXT_ROUND;
        }


        NEXT_ROUND:

        // check if any process missing deadline
        for (int i = 0; i < list_length(pcbs); i++){
            pcb_t* process = (pcb_t*) list_get_item(pcbs, i);
            int turnaround = time - process->entry_time;
            if (process->deadline == turnaround){
                log_pcb(process, LOG_DEADLINEMISS, time, output_fd);
            }
        }

        // if all processes is executed
        if (done >= total_process_count){
            break;
        }
        quantprog ++;
        time ++;
    }

    list_free(pcbs);

}

void pr_run_processes_uf(linked_node_t* processes, int output_fd){
    /* 
     * This function runs a list of processes stored as process_record_t
     * using the urgent first alrgorithm.
     * The output is printed to stdout and written to the output file.
     * The algorithm will check for which process has the most urgent deadline to meet
     * everyround, and execute that process until finished.
     * ARGUMENTS:
     *  - processes: any node from a linked list (list.h),
     *               in the type of process_record_t (process_record.h)
     *               The list of processes to be executed.
     *  - output_fd: an integer, the file descriptor of output file.
     */
}

pcb_t* pr_record_to_pcb(process_record_t* record){
    /* 
     * Converts a record to a pcb when the process arrives in to the system.
     * Also initialises fields such as remaining time and status to default value.
     * ARGUMENTS:
     *  - record: a process_record_t (see process_record.h) to be converted.
     * RETURN:
     *  - a pointer to the converted pcb_t (see process_runner.h) allocated in heap.
     */
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
    /* 
     * Takes in a list of processes in process_record_t* (see process_record.h)
     * and the time iteration of current round, returns a list of process_record_t*
     * that will arrive in this time iteration.
     * The newly created list will be in heap, while the content of the newly created list
     * points to the same addresses of the processes passed in through argument.
     * ARGUMENTS:
     *  - t: an integer, the time iteration the function need to find out for arriving processes.
     *  - processes: any node of the linked list storing the pointers
     *    pointing to processes in process_record_t.
     * RETURN:
     *  - a list containing the processes to be arrived in this time iteration.
     */
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