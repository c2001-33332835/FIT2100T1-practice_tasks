#ifndef LOGS_H
#define LOGS_H
#include "list.h"
#include "process_record.h"
#include "process_runner.h"

/* This is the library for logging into stdout and into file.
 * When using log_string, it is being logged to stdout as a replacement of printf
 * Multiple log level is provided such as error, warning and etc.
 * When using the LOG_STRERR log level/log type, message is printed to stderr instead
 * log_pcb and log_iteration is part of the simulation, which logs the time iteration and important events
 * When a process being logged is exitting using the LOG_EXIT type, the process execution summary 
 * is being written to the result file opened.
 * 
 * This is the header file for interface and intellisense 
 * The implementation of this file is in logs.c
 */

typedef enum {
    LOG_ARRIVE,
    LOG_START,
    LOG_SUSPENSION,
    LOG_RESUME,
    LOG_EXIT,
    LOG_DEADLINEMISS
} log_functional_type_t;

typedef enum {
    LOG_STRDFLT,
    LOG_STRWARN,
    LOG_STRERR,
    LOG_STRDBUG
} log_string_type_t;

void log_string(char* message, log_string_type_t log_type);
void log_iteration(int iteration);
void log_pcb(pcb_t* block, log_functional_type_t log_type, int t, int output_fd);

#endif