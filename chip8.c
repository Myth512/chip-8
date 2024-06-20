#include <stdio.h>

char RAM[4096];

int main(){
	RAM[0] = 45334516;
	printf("%d \n", RAM[0]);
	return 0;
}
