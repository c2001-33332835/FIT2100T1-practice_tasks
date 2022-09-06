#include <stdio.h>
#include <stdlib.h>

int main(void){
    printf("test1\n");
    execl("/usr/bin/echo", "echo", "hello", NULL);
    printf("test2\n");
    printf("test3\n");
    return 0;
}
