#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define PGSIZE 4096
#define SIZE 20

int
main(int argc, char *argv[]){
	//printf(1, "hello myMemTest, argc = %d \n", argc);
	int i, j;
	char *arr[SIZE];
	char input[10];

	for (i = 0; i < 12; ++i)
	{
		arr[i] = sbrk(PGSIZE);
	}
	printf(1, "called sbrk(PGSIZE) 12 times, press any key...\n");
	gets(input, 10);
	arr[12] = sbrk(PGSIZE);
	printf(1, "called sbrk(PGSIZE) for the 13th time, page replacement algorithm should have completed successfuly, press any key...\n");
	gets(input, 10);
	for (i = 0; i < 13; ++i) {
		for (j = 0; j < PGSIZE; j++)
			arr[i][j] = 'k';
	}
	printf(1, "Successfuly written data to all pages. exiting...\n");

	// for (i = 0; i < SIZE; ++i)
	// {
	// 	free(arr[i]);
	// }
	// printf(1, "should have freed all memory allocated by sbrk(), check with ^p\nPress any key to exit function.\n");
	//
	// gets(input, 10);

	exit();
	//return 0;
}
