#include <stdlib.h>
#include <string.h>
#include "list.h"

linked_node_t* str_split(char* source, char deliminater){
    linked_node_t* result = list_create_empty();
    char buffer[strlen(source) + 1];
    buffer[strlen(source)] = "\0";
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

int str_ordercmp(char* x, char* y){
    // 0 = eq, 1 = x > y, -1 = y > x

    int lx = strlen(x); // length of x
    int ly = strlen(y); // length of y
    int cmpmax = (lx > ly) ? ly : lx; // shorter value as maximum cmp length
    
    for (int i = 0; i < cmpmax; i ++){
        // compare each char, until x > y or y > x
        char cx = x[i];
        char cy = y[i];
        if (cx == cy){
            continue;
        }
        return (cx > cy) ? 1 : -1;
    }

    // if all char with in maximum cmp length is the same, 
    // longer string is greater

    if (lx == ly) return 0;
    return (lx > ly) ? 1 : -1;
}

void int_count_digits(int n, int* result){
    /* 
     * This funtion contains code from the following URL:
     * https://stackoverflow.com/questions/3068397/finding-the-length-of-an-integer-in-c
     * By Jordan Lewis, 18th of June, 2010
     * 
     * Thus the content of this function does not license under MIT LICENSE.
     * Jordan Lewis owns the copyright of the content of this function.
     */

    *result  = floor(log10(abs(n))) + 1;
}