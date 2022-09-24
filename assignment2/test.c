#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define castpointerval(T, V) *((T*)(V))
#define malloctype(T, M) ((T*) malloc(M * sizeof(T)))
#define in_range(I, MAX) (int I = 0; I < MAX; I++)

int main(void){

    linked_node_t* list_a = list_create_size(10);

    for in_range(i, 10) {
        int* data = malloctype(int, 1);
        *data = i;
        list_set_item(list_a, i, data);
    }

    list_remove_last(list_a);

    // printf("%d", castpointerval(int ,list_a->content));
    printf("%d", castpointerval(int ,list_get_item(list_a, 100)));

    return 0;

    list_free(list_a);
}