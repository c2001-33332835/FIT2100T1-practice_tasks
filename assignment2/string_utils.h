#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include "list.h"

/* This is a library for custom string utilities.
 * The utilities includes:
 *  - Splitting a string into a linked list by a deliminater.
 *    (The linked list is defined and decleared in list.h and list.c)
 *  - Counting integer by its length (kind of like counting the length after converting to string)
 *  - Compare the alphabetical order of 2 string.
 * 
 * This is the header file for interface and intellisense 
 * The implementation of this file is in string_utils.c
 */

linked_node_t* str_split(char* source, char deliminator);
int str_ordercmp(char* x, char* y);
void int_count_digits(int n, int* result);

#endif