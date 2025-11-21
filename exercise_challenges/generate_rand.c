#include <stdlib.h>
#include <stdio.h>

char flag[] = "flag{this_challenge_is_from_htb}";
int main(){
	for(int i = 0; i < sizeof(flag); i++){
		srand(flag[i]);
		int r = rand();
		printf("%d, ", r);
	}
	printf("\n");
}
