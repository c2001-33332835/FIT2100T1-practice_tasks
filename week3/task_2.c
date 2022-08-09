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

// this program calculates the sum of arguments
int main(int argc, char *argv[]){
    // define variables
    int result = 0;

    for (int i = 0; i < argc; i ++){
        result += parse_int(argv[i]);
    }

    printf("%d", result);
}
