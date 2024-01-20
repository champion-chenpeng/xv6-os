// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem[NCPU];

void
kinit()
{
  int cpui;
  char kmemname[] = "kmem_000";
  push_off();
  cpui = cpuid();

  snprintf(kmemname, sizeof(kmemname), "kmem_%d", cpui);
  initlock(&kmem[cpui].lock, kmemname);
  freerange(end, (void*)PHYSTOP);
  pop_off();
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  int cpui;
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  push_off();
  cpui = cpuid();

  acquire(&kmem[cpui].lock);
  r->next = kmem[cpui].freelist;
  kmem[cpui].freelist = r;
  release(&kmem[cpui].lock);

  pop_off();
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  int cpui;
  struct run *r;
  push_off();
  cpui = cpuid();

  acquire(&kmem[cpui].lock);
  r = kmem[cpui].freelist;
  if(r) {
    kmem[cpui].freelist = r->next;
    release(&kmem[cpui].lock);
  } else { // steal from other cpus
    	release(&kmem[cpui].lock);
		for (int i = 1; i < NCPU; i++) {
			int cpui_t = (cpui + i) % NCPU;
			acquire(&kmem[cpui_t].lock);
			r = kmem[cpui_t].freelist;
			if (r) {
				kmem[cpui_t].freelist = r->next;
				release(&kmem[cpui_t].lock);
				break;
			}
			release(&kmem[cpui_t].lock);
		}
	}
  pop_off();

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
