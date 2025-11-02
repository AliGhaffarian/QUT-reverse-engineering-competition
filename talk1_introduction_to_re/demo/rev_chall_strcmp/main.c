// reverse_ctf.c
// flag: flag{password_password}
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
	char input[128] = {0};

	int rbyte = read(STDIN_FILENO, input, sizeof(input));

	if(!rbyte){
		puts("false");
		return 0;
	}

	if(input[rbyte - 1] == '\n')
			input[rbyte - 1] = 0;

	if(strcmp("flag{password_password}", input))
		puts("false");
	else
		puts("true");

	return 0;
}

