#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H
#include "list.h"

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
void pr_sort_records(linked_node_t* records, process_record_field_t sort_by, int asc);
void* pr_print_record(process_record_t* record);

#endif