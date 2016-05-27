// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "kalloc.h"

struct physPagesCounts physPagesCounts;

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;


// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  freerange(vstart, vend);

  // physPagesCounts is a struct defined in kalloc.h to hold info needed to cumpute percent of free physcal pages
  // all physical pages allocated to the kernel's allocator's "freelist" are allocated in kinit1 & kinit2
  // here we update the # of pages inserted to free list in kinit1
  physPagesCounts.initPagesNo = (PGROUNDDOWN((uint)vend) - PGROUNDUP((uint)vstart)) / PGSIZE;
  cprintf("physPagesCounts->initPagesNo = %d\n", physPagesCounts.initPagesNo );
  //cprintf("physPagesCounts->currentFreePagesNo = %d\n", physPagesCounts.currentFreePagesNo );
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  // update the # of pages inserted to free list in kinit2
  physPagesCounts.initPagesNo += (PGROUNDDOWN((uint)vend) - PGROUNDUP((uint)vstart)) / PGSIZE;
  
  //cprintf("physPagesCounts->initPagesNo = %d\n", physPagesCounts.initPagesNo );
  //cprintf("physPagesCounts->currentFreePagesNo = %d\n", physPagesCounts.currentFreePagesNo );
  //cprintf("percent of free physical pages: %d\n", physPagesCounts.currentFreePagesNo * 100 / physPagesCounts.initPagesNo);

  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}

//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || v2p(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  physPagesCounts.currentFreePagesNo++;
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
    physPagesCounts.currentFreePagesNo--;
  }
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}
