#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "list.h"
#include "string_utils.h"
#include "process_record.h"
#include "process_runner.h"
#include "logs.h"

#define FALLBACK_SOURCE "processes.txt"
#define FALLBACK_REUSLT "results-2.txt"
#define RR_TIMEQUANT 2

/* This is the main file
 * which contains the entry point for simulation process for the RR algorithm
 * This program will load source file, either from a file specified in argv[1]
 * or the fallback file of "processes.txt"
 * The simulation result will be printed to stdout
 * a file will be created to store the summary (task1_33332835)
 * 
 * Student Name/ID: Xuanao Zhao / 33332835
 * Copyright: (c) 2022 Xuanao Zhao, MIT License Applied
 * Latest Modification Date: 8th of OCT, 2022
 */

int load_file(linked_node_t** result, char* filename){
    /* 1 = success, 0 = failed */

    // log an info message
    char* base_msg = "Loading process(es) from ";
    char msg[strlen(base_msg) + strlen(filename) + 1];
    sprintf(msg, "%s%s", base_msg, filename);
    log_string(msg, LOG_STRDFLT);
    
    // open file
    int fd = open(filename, O_RDONLY);
    if (fd == -1){
        // if failed, generate error message and return 0;
        char* base_err = "Error while loading file %s: No such file or permission denied.";
        char err[strlen(base_err) + strlen(filename) + 1];
        sprintf(err, base_err, filename);
        log_string(err, LOG_STRERR);
        return 0;
    }

    // read and parse file
    *result = pr_read_source_file(fd);

    // close file after reading.
    close(fd);
    return 1;
}

int open_output_file(int* fd){
    /* 1 = success, 0 = failed */
    
    // open file
    *fd = open(FALLBACK_REUSLT, O_TRUNC | O_CREAT | O_WRONLY, 0664);
    if (*fd == -1){
        // if failed, generate error message and return 0;
        char* base_err = "Error while loading result file %s: Permission denied.";
        char err[strlen(base_err) + strlen(FALLBACK_REUSLT) + 1];
        sprintf(err, base_err, FALLBACK_REUSLT);
        log_string(err, LOG_STRERR);
        return 0;
    }

    return 1;
}

int main(int argc, char* argv[]){
    // get filename from argv if available
    linked_node_t* processes;
    int file_result;
    if (argc > 1){
        file_result = load_file(&processes, argv[1]);
    }
    else{
        file_result = load_file(&processes, FALLBACK_SOURCE);
    }

    // load file, if failed, exit with 1.
    if (!file_result){
        return 1;
    }

    // load output file, if failed, exit with 1.
    int output_fd;
    if (!open_output_file(&output_fd)){
        return 1;
    }

    char* base_schedule_msg = "Scheduling %d process(es) with Round robin (RR) Algorithm, with a time quantum of %d.";
    int process_count_length;
    int_count_digits(list_length(processes), &process_count_length);
    int tq_length;
    int_count_digits(RR_TIMEQUANT, &tq_length);
    char schedule_msg[strlen(base_schedule_msg) + tq_length + process_count_length + 1];
    sprintf(schedule_msg, base_schedule_msg, list_length(processes), RR_TIMEQUANT);
    log_string(schedule_msg, LOG_STRDFLT);

    // run round robin algorithm
    pr_run_processes_rr(processes, RR_TIMEQUANT, output_fd);

    log_string("All processes exited.", LOG_STRDFLT);

    // free and close resources after execution
    list_free(processes);

    // return 0 for success execution
    return 0;
}