#include <fcntl.h>
#include <stdio.h>

//filename secret_filename__
//expected input: shifted_random_string
#define string_len (sizeof(shifted) / sizeof(int))
int main(){
	int shifted[] = {0x1cc,0x1a0,0x1a4,0x198,0x1d0,0x194,0x190,0x17c,0x1c8,0x184,0x1b8,0x190,0x1bc,0x1b4,0x17c,0x1cc,0x1d0,0x1c8,0x1a4,0x1b8,0x19c};
	int xor_key = 0x22;
	int filename_xored[] = {0x51,0x47,0x41,0x50,0x47,0x56,0x7d,0x44,0x4b,0x4e,0x47,0x4c,0x43,0x4f,0x47,0x7d,0x7d, 0x22};
	char filename_plain[sizeof(filename_xored) / sizeof(int)] = {0};
	char read_buff[256] = {0};

	for ( int i = 0; i < sizeof(filename_xored); i++)
		filename_plain[i] = filename_xored[i] ^ xor_key;
	int fd = open(filename_plain, O_RDONLY, 0);
	if(fd == -1){
		printf("file not found\n");
		return 1;
	}
	int read_bytes = read(fd, read_buff, string_len);
	for (int i = 0; i < string_len; i++){
		if (((int)read_buff[i] << 2) != shifted[i]){
			puts("wrong");
			return 1;
		}
	}
	puts("right");
	return 0;
}
