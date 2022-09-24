#include <stdio.h>
#include <pthread.h>
#include <string.h>

int sum = 0;

void* add(int i){
    // args[0] address of sum
    // args[1] address of iteration
    
    pthread_t ctid = pthread_self();
    
    sum += i;

    printf("[Thread@%lu] i=%d, sum=%d\n", ctid, i, sum);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int n = atoi(argv[1]);
    pthread_t threads[n];
    int sum = 0;
    pthread_t mtid = pthread_self();

    pthread_t* taddr = threads;
    for (int i = 0; i < n; i++){
        pthread_create(taddr, NULL, add, i);
        printf("[Main@%lu] new_thread=%lu, i=%d, sum=%d\n", mtid, *taddr, i, sum);
        taddr++;
    }

    for (int i = 0; i < n; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
