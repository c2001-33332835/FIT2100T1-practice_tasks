#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "process_record.h"
#include "list.h"
#include "string_utils.h"

/* This is the library to read and parse source files into process records.
 * Thie library works when the file is already opened using the low level open,
 * and accepts file descriptor to read files.
 * All records are being parsed into process_record_t, a struct type, and stored into 
 * a list defined and decleared in list.h and list.c. All data parsed is stored in heap.
 */

linked_node_t* pr_read_source_file(int fd){
    /* 
     * this function reads the source file containig process information,
     * parse each of them into a list of process_record_t* (see process_record.h)
     * ARGUMENTS:
     *  - fd: an integer of the opened sourcefile's file descriptor
     * RETURN:
     *  - a node from the linked list sotring the result of parsing, in the type of process_record_t*
     *    all nodes and records are stored in heap
     */
    
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
        process_record_t* record = pr_parse_record_line(line);
        list_append_node(process_records, record);
    }

    free(raw);
    list_free(lines);

    return process_records;
}

process_record_t* pr_parse_record_line(char* line){
    /* 
     * this function takes in a line of source file, 
     * parse the line to store into a process_record_t sored in heap (see process_record.h)
     * ARGUMENTS:
     *  - line: a pointer to a string containing the raw line of source file.
     * RETURN:
     *  - a pointer to the parsed process_record_t stored in heap.
     */

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

void pr_print_record(process_record_t* record){
    char* name = record->process_name;
    int arrive = record->arrive_time;
    int service = record->service_time;
    int deadline = record->deadline;

    printf("<process_record_t n='%s' a=%d s=%d d=%d>\n", name, arrive, service, deadline);
}