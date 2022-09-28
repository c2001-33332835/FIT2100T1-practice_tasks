#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> // debug
#include "process_runner.h"
#include "list.h"
#include "process_record.h"

void run_processes(linked_node_t* processes){
    int finished = 0;
    unsigned time_iteration;
    unsigned process_count = list_length(processes);
    linked_node_t* pcbs = list_create_empty();
    for (time_iteration = 0; finished < process_count; time_iteration++){
        printf("In iteration %u\n", time_iteration);
        
        // convert records into pcbs
        linked_node_t* next = next_processes(time_iteration, processes);
        for (int i = 0; i < list_length(next); i++){
            pcb_t* new_process = (pcb_t*) malloc(sizeof(pcb_t));
            process_record_t* new_process_record = (process_record_t*) list_get_item(next, i);
            new_process->entry_time = new_process_record->arrive_time;
            new_process->remaining_time = new_process_record->service_time;
            new_process->service_time = new_process_record->service_time;
            new_process->status = READY;
            list_append_node(pcbs, new_process);
            pr_print_record(new_process_record);
        }
        list_free_nodes(next);

        sleep(1);
    }

    list_free(pcbs);
}

linked_node_t* next_processes(int t, linked_node_t* processes){
    linked_node_t* next = list_create_empty();
    for (int i = 0; i < list_length(processes); i++){
        process_record_t* record = (process_record_t*) list_get_item(processes, i);
        if (record->arrive_time == t){
            list_append_node(next, record);
        }
    }
    return next;
}