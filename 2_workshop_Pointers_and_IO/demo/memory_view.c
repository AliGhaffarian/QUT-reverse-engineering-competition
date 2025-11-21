#include <stdio.h>
int main(){
char buffer[128];
	for( int i = 0; i < 128; i++ ){
		printf("|%p|\t%#04hhx\t|\n", buffer + i, *(buffer + i));
		puts("---------------------------------");
	}
}
