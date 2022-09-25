#include <stdlib.h>
#include <string.h>
#include "list.h"

linked_node_t* str_split(char* source, char deliminater){
    linked_node_t* result = list_create_empty();
    char buffer[strlen(source)];
    strcpy(buffer, "");
    char c[2];
    c[1] = '\0';
    for (int i = 0; i <= strlen(source); i++){
        c[0] = source[i];
        if (c[0] == deliminater || c[0] == '\0'){
            // dump buffer to heap and append to list
            char* segment = (char*) malloc((strlen(buffer)+1) * sizeof(char));
            strcpy(segment, buffer);
            list_append_node(result, segment);
            strcpy(buffer, "");
            continue;
        }
        strcat(buffer, c);
    }
    return result;
}