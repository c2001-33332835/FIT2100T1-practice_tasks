#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H
#include "list.h"

typedef struct process_record {
    char process_name[11];
    int arrive_time;
    int service_time;
    int deadline;
} process_record_t;

linked_node_t* pf_read_source_file(int fd);
process_record_t* pf_parse_record_line(char* line);
char* pf_record_to_str(process_record_t* record);

#endif