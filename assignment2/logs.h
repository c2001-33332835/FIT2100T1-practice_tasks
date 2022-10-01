#ifndef LOGS_H
#define LOGS_H
#include "list.h"
#include "process_record.h"
#include "process_runner.h"

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