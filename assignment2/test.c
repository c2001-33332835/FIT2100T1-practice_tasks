#include <stdio.h>

struct a{
    int b;
};

typedef struct a a_t;

int main(void){
    a_t c;
    c.b = 0;

    struct a d = c;
    d.b = 1;

    printf("%d", c.b);
    printf("%d", d.b);
    return 0;
}