#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define PGSIZE 4096

int
main(int argc, char *argv[]){
	int i, j, pgNum = 12, times = 27, writeTimes = 5;
	char *arr[27];
	char input[10];

	//sbrk(-1);
	//printf(1, "Called sbrk(-1)\nPress any key...\n");
	printf(1, "hello.. Press any key to allocate %d pages...\n", times);
	gets(input, 10);

	for (i = 0; i < times; ++i)
	{
		arr[i] = sbrk(PGSIZE);
		printf(1, "arr[%d]=0x%x\n", i, arr[i]);
//		if (i % 5 == 4) 
//		{
//			printf(1, "called sbrk %d times.. again?\n", i);
//			gets(input, 10);
//		}
	}
	//printf(1, "calling sbrk(-1) first...\n");
	//sbrk(-1);
//	printf(1, "\n==== DEBUG === then enter key to add page and swap file...\n");
	printf(1, "Called sbrk(PGSIZE) %d times. \nPress any key to write to memory...\n", times);
	gets(input, 10);

	//arr[i] = sbrk(PGSIZE);
	printf(1, "writing to %d different pages %d times...\n", pgNum, writeTimes);
	for (j = 0; j < writeTimes; j += 1)
		for (i = 12; i < 12 + pgNum ; ++i) {
			printf(1, "writing to page No. %d, addr 0x%x\n", i, arr[i]);
			arr[i][j] = 'k';
		}
	//sbrk(-1);
	printf(1, "Written to all %d pages successfuly.\nPress any key to exit...\n", times);
	gets(input, 10);
	exit();
}
