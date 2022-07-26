#include <stdio.h>

int main(){
	int digitCount = 4;
	char digitsRaw[digitCount];
	int digitsRev[digitCount];
	
	for (int i = 0; i < digitCount; i++){
		scanf("%c", &digitsRaw[i]);
	}

	for (int i = 0; i < digitCount; i++){
		printf("%c", digitsRaw[(digitCount - i) - 1]);
		digitsRev[i] = digitsRaw[(digitCount - i) - 1] - 43;
	}

}
