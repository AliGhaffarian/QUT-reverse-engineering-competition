#include <stdlib.h>
#include <stdio.h>

int compare_table[] = {371, 391, 355, 374, 440, 417, 378, 381, 414, 348, 361, 378, 355, 391, 391, 368, 397, 374, 368, 348, 381, 414, 348, 371, 410, 401, 394, 348, 378, 417, 358, 446, 38};

int main(){
	char input[128];
	scanf("%s", input);
	for(int i = 0; i < sizeof(compare_table) / sizeof(int); i++){
		srand(input[i]);
		int random = rand();
		if(random != compare_table[i]){
			puts("wrong");
			exit(1);
		}
		//printf("%c, %d ? %d\n", input[i], random, compare_table[i]);
	}
	puts("right");
	exit(0);
}
