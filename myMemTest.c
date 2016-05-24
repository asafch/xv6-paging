#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define PGSIZE 4096

int
main(int argc, char *argv[]){
	//printf(1, "hello myMemTest, argc = %d \n", argc);
	int i, size = 5;
	char *arr[size];
	char input[10];

	for (i = 0; i < size; ++i)
	{
		arr[i] = (char*) malloc(PGSIZE);
	}

	printf(1, "called malloc(PGSIZE) %d times. press any key to free the memory.\n", size);
	gets(input, 10);

	for (i = 0; i < size; ++i)
	{
		free(arr[i]);
	}
	printf(1, "should have freed all memory allocated by malloc()s. check with ^p \npress any key to exit function.\n");

	gets(input, 10);

	exit();
	//return 0;
}
