#include <stdio.h>

int main() {
	int number = 0x55443322;
	int *ptr_number = &number;
	char *c_ptr_number = (char *)&number;
	void *v_ptr_number = (void *)&number;

	printf("ptr_number = %p\n", ptr_number);
	printf("ptr_number + 1 = %p\n", ptr_number + 1);
	puts("-------------------");
	printf("c_ptr_number = %p\n", c_ptr_number);
	printf("c_ptr_number + 1 = %p\n", c_ptr_number + 1);
	puts("-------------------");
	printf("v_ptr_number = %p\n", v_ptr_number);
	printf("v_ptr_number + 1 = %p\n", v_ptr_number + 1);
	puts("-------------------");

	printf("number 0x%x\n", number);
	printf("deref ptr_number 0x%x\n", *ptr_number);

	puts("-------------------");
	printf("deref c_ptr_number + 0 = 0x%x\n", *c_ptr_number );
	c_ptr_number++;
	printf("deref c_ptr_number + 1 = 0x%x\n", *c_ptr_number );
	c_ptr_number++;
	printf("deref c_ptr_number + 2 = 0x%x\n", *c_ptr_number );
	c_ptr_number++;
	printf("deref c_ptr_number + 3 = 0x%x\n", *c_ptr_number );
}
