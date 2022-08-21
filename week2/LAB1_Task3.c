/* task2.c */
#include <stdio.h>

int main(){
	char string[10] = "N/A"; // Initialise our string

	printf("Enter a word, up to 10 characters long: ");
	scanf("%s", &string);

	printf("You entered %s\n", &string);
}
