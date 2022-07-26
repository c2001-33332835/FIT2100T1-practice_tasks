#include <stdio.h>

int main(){
	char fname[20];
	char lname[20];

	scanf("%s", &fname);
	fflush(stdin);
	scanf("%s", &lname);
	fflush(stdin);

	printf("%c%c",fname[0], lname[0]);

	return 0;
}
