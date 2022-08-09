#include <stdio.h>

// as described by question
int* find_largest(int the_array[], int num_elements){
    int* result = &the_array[0];
    int max = the_array[0];
    for (int i = 0; i < num_elements; i++){
        int* current = &the_array[i];
        if (the_array[i] > max){
            max = the_array[i];
            result = &the_array[i];
        }
    }
    return result;
}

// this program calls the function defined above as a demo
int main(void){
    int test[] = {1,2,3,4,5,9,8,7,6};
    int *result = find_largest(test, 9);
    printf("Test 1: {1,2,3,4,5,9,8,7,6} => %d", *result);
}
