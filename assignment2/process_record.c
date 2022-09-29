#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "process_record.h"
#include "list.h"
#include "string_utils.h"

linked_node_t* pr_read_source_file(int fd){
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
        process_record_t* record = pr_parse_record_line(line);
        list_append_node(process_records, record);
    }

    // free all temporary resources before return
    free(raw);
    list_free(lines);

    return process_records;
}

process_record_t* pr_parse_record_line(char* line){
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

void pr_sort_records(linked_node_t* records, process_record_field_t sort_by, int asc){
    // bubble sort
    unsigned record_length = list_length(records);
    for (int end = record_length; end > 0; end--){
        for (int i = 0; i < end; i++){
            process_record_t* r1 = (process_record_t*)(list_get_item(records, i));
            process_record_t* r2 = (process_record_t*)(list_get_item(records, i+1));
            
            int swap = 0;
            switch (sort_by){
                case PR_NAME:
                    swap = str_ordercmp(r1->process_name, r2->process_name) == 1;
                case PR_ARRIVE:
                    swap = r1->arrive_time > r2->arrive_time;
                case PR_SERVICE:
                    swap = r1->service_time > r2->service_time;
                case PR_DEADLINE:
                    swap = r1->deadline > r2->deadline;
                default:
                    break;
            }

            if (swap){
                // swap place if required.
                linked_node_t* n1 = list_get_node(records, i);
                linked_node_t* n2 = list_get_node(records, i+1);
                
                void* temp = n1->content;
                n1->content = n2->content;
                n2->content = temp;
            }
        }
    }
}

void pr_print_record(process_record_t* record){
    char* name = record->process_name;
    int arrive = record->arrive_time;
    int service = record->service_time;
    int deadline = record->deadline;

    printf("<process_record_t n='%s' a=%d s=%d d=%d>\n", name, arrive, service, deadline);
}