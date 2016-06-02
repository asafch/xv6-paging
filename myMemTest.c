#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define PGSIZE 4096

int
main(int argc, char *argv[]){

	#if FIFO

	int i, j;
	char *arr[14];
	char input[10];
	// Allocate all remaining 12 physical pages
	for (i = 0; i < 12; ++i) {
		arr[i] = sbrk(PGSIZE);
		printf(1, "arr[%d]=0x%x\n", i, arr[i]);
	}
	printf(1, "Called sbrk(PGSIZE) 12 times - all physical pages taken.\nPress any key...\n");
	gets(input, 10);

	/*
	Allocate page 15.
	This allocation would cause page 0 to move to the swap file, but upon returning
	to user space, a PGFLT would occur and pages 0,1 will be hot-swapped.
	Afterwards, page 1 is in the swap file, the rest are in memory.
	*/
	arr[12] = sbrk(PGSIZE);
	printf(1, "arr[12]=0x%x\n", arr[12]);
	printf(1, "Called sbrk(PGSIZE) for the 13th time, a page fault should occur and one page in swap file.\nPress any key...\n");
	gets(input, 10);

	/*
	Allocate page 16.
	This would cause page 2 to move to the swap file, but since it contains the
	user stack, it would be hot-swapped with page 3.
	Afterwards, pages 1 & 3 are in the swap file, the rest are in memory.
	*/
	arr[13] = sbrk(PGSIZE);
	printf(1, "arr[13]=0x%x\n", arr[13]);
	printf(1, "Called sbrk(PGSIZE) for the 14th time, a page fault should occur and two pages in swap file.\nPress any key...\n");
	gets(input, 10);

	/*
	Access page 3, causing a PGFLT, since it is in the swap file. It would be
	hot-swapped with page 4. Page 4 is accessed next, so another PGFLT is invoked,
	and this process repeats a total of 5 times.
	*/
	for (i = 0; i < 5; i++) {
		for (j = 0; j < PGSIZE; j++)
			arr[i][j] = 'k';
	}
	printf(1, "5 page faults should have occurred.\nPress any key...\n");
	gets(input, 10);

	if (fork() == 0) {
		printf(1, "Child code running.\n");
		printf(1, "View statistics for pid %d, then press any key...\n", getpid());
		gets(input, 10);

		/*
		The purpose of this write is to create a PGFLT in the child process, and
		verify that it is caught and handled properly.
		*/
		arr[5][0] = 't';
		printf(1, "A page fault should have occurred for page 8.\nPress any key to exit the child code.\n");
		gets(input, 10);

		exit();
	}
	else {
		wait();

		/*
		Deallocate all the pages.
		*/
		sbrk(-14 * PGSIZE);
		printf(1, "Deallocated all extra pages.\nPress any key to exit the father code.\n");
		gets(input, 10);
	}

	#elif SCFIFO


	#elif NFU


	#else
	char* arr[50];
	int i = 50;
	printf(1, "Commencing user test for default paging policy.\nNo page faults should occur.\n");
	for (i = 0; i < 50; i++) {
		arr[i] = sbrk(PGSIZE);
		printf(1, "arr[%d]=0x%x\n", i, arr[i]);
	}
	#endif
	exit();
}
