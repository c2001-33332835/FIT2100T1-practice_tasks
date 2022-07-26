#define PI 3.14159265358979
#include <stdio.h>

float addition(int a, int b){ return a+b; }
float subtraction(int a, int b) { return a-b; }
float multiplication(int a, int b) { return a*b; }
float division(int a, int b) { return a/b; }

int main(){
	int a, b;
	scanf("%d", &a);
	scanf("%d", &b);

	printf("%f\n", addition(a,b));
	printf("%f\n", subtraction(a,b));
	printf("%f\n", multiplication(a,b));
	printf("%f\n", division(a,b));

	return 0;
}
