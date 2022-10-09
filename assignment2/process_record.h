#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H
#include "list.h"

/* This is the library to read and parse source files into process records.
 * Thie library works when the file is already opened using the low level open,
 * and accepts file descriptor to read files.
 * All records are being parsed into process_record_t, a struct type, and stored into 
 * a list defined and decleared in list.h and list.c. All data parsed is stored in heap.
 *
 * This is the header file for interface and intellisense 
 * The implementation of this file is in process_record.c
 */

typedef struct process_record {
    char process_name[11];
    int arrive_time;
    int service_time;
    int deadline;
} process_record_t;

typedef enum {
    PR_NAME, PR_ARRIVE, PR_SERVICE, PR_DEADLINE
} process_record_field_t;

linked_node_t* pr_read_source_file(int fd);
process_record_t* pr_parse_record_line(char* line);
void pr_print_record(process_record_t* record);

#endif