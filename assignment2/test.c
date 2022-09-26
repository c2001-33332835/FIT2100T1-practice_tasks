#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include "string_utils.h"
#include "process_record.h"

#define castpointerval(T, V) *((T*)(V))
#define castpointer(T, V) ((T*)(V))
#define malloctype(T, M) ((T*) malloc(M * sizeof(T)))
#define in_range(I, MAX) (int I = 0; I < MAX; I++)

int main(void){
    int fd = open("processes.txt", O_RDONLY);
    linked_node_t* records = pr_read_source_file(fd);

    for (int i = 0; i < list_length(records); i++){
        process_record_t* record = castpointer(process_record_t, list_get_item(records, i));
        char* c = pr_print_record(record);
    }
}