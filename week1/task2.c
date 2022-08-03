#define PI 3.14159265358979
#include <stdio.h>

int main(){
	float r;

	scanf("%f", &r);
	printf("Surface area = %f\n", 4 * PI * r * r);
	printf("Volume area = %f\n", (4 / 3) * PI * r * r * r);
	return 0;
}
