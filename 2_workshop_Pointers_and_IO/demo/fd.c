#include <fcntl.h>
#include <stdio.h>
int main(){
	char buff[128] = {0};
	int fd = open("./hello_world.txt", O_RDONLY, 0);
	printf("fd = %d\n", fd);
	int read_bytes = read(fd, buff, sizeof(buff));
	printf("read_byes = %d\n", read_bytes);
	printf("buff = 0x%p\n", buff);
	write(1 , buff, read_bytes);
}
