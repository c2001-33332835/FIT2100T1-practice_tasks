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
    int result1 = str_ordercmp("P1", "P2");
    int result2 = str_ordercmp("P2", "P1");
    int result3 = str_ordercmp("P1", "P1");
    printf("P1, P2 %d\n", result1);
    printf("P2, P1 %d\n", result2);
    printf("P1, P1 %d\n", result3);
}