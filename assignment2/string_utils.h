#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include "list.h"

linked_node_t* str_split(char* source, char deliminator);
int str_ordercmp(char* x, char* y);

#endif