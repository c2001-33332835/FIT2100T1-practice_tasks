#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	int digitCount = 6;
	int digits = 0;

	for (int i = 0; i < digitCount; i ++){
		int temp = rand() % 10;
		digits += temp * pow(10, i);
	}

	printf("%d", digits);
}
