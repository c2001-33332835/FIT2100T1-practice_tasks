#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "process_file.h"
#include "list.h"
#include "string_utils.h"

linked_node_t* pf_read_source_file(int fd){
    /**/
    
    // read file content into raw (a temporary file storage)
    char* raw = (char*) malloc(sizeof(char) * 2);
    int char_count = 0;
    char buf[1];
    while (1){
        int success_size = read(fd, buf, 1);
        if (!success_size){
            // if no more to read
            break;
        }
        
        raw[char_count] = buf[0];
        raw = (char*) realloc(raw, sizeof(char) * (char_count + 2));
        char_count ++;
    }

    linked_node_t* lines = str_split(raw, '\n');

    // parse each line
    linked_node_t* process_records = list_create_empty();
    for (int i = 0; i < list_length(lines); i++){
        char* line = (char*) list_get_item(lines, i);
        process_record_t* record = pf_parse_record_line(line);
        list_append_node(process_records, record);
    }

    // free all temporary resources before return
    free(raw);
    list_free(lines);

    return process_records;
}

process_record_t* pf_parse_record_line(char* line){
    /**/

    // split line with spaces
    linked_node_t* columns = str_split(line, ' ');
    
    // extract values for all columns
    char* process_name = (char*)list_get_item(columns, 0);
    char* arrive_time_raw = (char*)list_get_item(columns, 1);
    char* service_time_raw = (char*)list_get_item(columns, 2);
    char* deadline_raw = (char*)list_get_item(columns, 3);
    
    int arrive_time = atoi(arrive_time_raw);
    int service_time = atoi(service_time_raw);
    int deadline = atoi(deadline_raw);

    // assign the record
    process_record_t* record = (process_record_t*) malloc(sizeof(process_record_t));
    strcpy(record->process_name, process_name);
    record->arrive_time = arrive_time;
    record->service_time = service_time;
    record->deadline = deadline;
    
    // free temporary resources before return
    list_free(columns);

    return record;
}

void* pf_print_record(process_record_t* record){
    char* name = record->process_name;
    int arrive = record->arrive_time;
    int service = record->service_time;
    int deadline = record->deadline;

    printf("<process_record_t n='%s' a=%d s=%d d=%d>\n", name, arrive, service, deadline);
}