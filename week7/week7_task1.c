#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if (fork() == 0) {
        execl("/usr/bin/echo", "echo", argv[1], NULL);
    }
    else {
        execl("/usr/bin/less", "less", argv[2], NULL);
    }

    return 0;
}
