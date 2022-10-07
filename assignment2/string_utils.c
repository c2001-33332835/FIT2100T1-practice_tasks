#include <stdlib.h>
#include <string.h>
#include "list.h"

linked_node_t* str_split(char* source, char deliminater){
    /* Takes in a pointer to a string, split the string by deliminator
     * stores the splitted string in a list, in the heap.
     * a node from the result list is returned as linked_node_t (see list.h)
     */

    // result will be in the form of list
    linked_node_t* result = list_create_empty();

    // create a buffer to temporarly store the chars and is set to empty string at first.
    char buffer[strlen(source) + 1];
    buffer[strlen(source)] = "\0";
    strcpy(buffer, "");

    // create a buffer to temporarly store a char for deliminater detection.
    char c[2];
    c[1] = '\0';

    for (int i = 0; i <= strlen(source); i++){
        // for each char, if the char is deliminator or null (end of string),
        // store the buffer to heap and stores the heap to the result list.
        // then resets the heap.
        // if not, add it to the buffer.
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
    /* Takes in 2 pointers to strings, x and y
     * Compares the 2 strings by alphabetically order
     * an result represented by integer is returned
     * if return value is 0, 2 string is euqal
     * if 1, x has a higher order compared to y
     * if -1, x has a lower order compared to y
     * if the first char is the same, compare the 2nd char and etc.
     */

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
    /* Accepts an integer n, and a pointer to the result variable.
     * The count of the digits is computated, while the sign is ignored
     * (only measure the numeric part, not the negative sign if it has one).
     * stores the result into the variable pointed by result pointer.
     */
    
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
