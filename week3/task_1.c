#include <stdio.h>
#include <string.h>
#include <math.h>

// function for parsing char to int
int parse_int(char string[]){
    int result = 0;
    int length = strlen(string);
    int index = 0;
    for (int i = 0; i < length; i++){
        char parsing = string[length - i - 1];
        int parsed = parsing - 48;

        if (parsed < 0 || parsed > 9){
            continue;
        }

        result += parsed * pow(10, index);
        index ++;
    }
    return result;
}

// this program calculates LCM of 2 numebrs
int main(int argc, char *argv[]){
    // check argument count
    if (argc < 3){
        fprintf( stderr, "%s: 2 arguments required, %d provided\n", argv[0], argc - 1);
        return 1;
    }

    // initialise the 2 numbers
    int raw_1 = parse_int(argv[1]);
    int raw_2 = parse_int(argv[2]);

    // sort and make sure raw_1 < raw_2
    if (raw_1 > raw_2){
        int temp = raw_1;
        raw_1 = raw_2;
        raw_2 = temp;
    }

    int result = raw_2;
    while (result % raw_1 > 0){
        result += raw_2;
    }

    printf("%d", result);
}
