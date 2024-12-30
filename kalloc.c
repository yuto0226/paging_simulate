// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "riscv.h"
#include "memorylayout.h"
#include "debug.h"

extern char end[];  // kernel end physical address

struct run {
  struct run* next;
};

// free frame list
struct {
  // lock unimplemented
  struct run* freelist;
} kmem;

// 初始化物理記憶體
void kinit() {
  // 初始化空閒記憶體列表
  struct run *r;
  char *p = (char*)PGROUNDUP((uint64)end);
  
  // 將所有頁面加入空閒列表
  for(; p + PGSIZE <= (char*)PHYSTOP; p += PGSIZE) {
    r = (struct run*)p;
    r->next = kmem.freelist;
    kmem.freelist = r;
  }
}

// 釋放物理記憶體
// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa) {
  struct run* r;
  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  r = (struct run*)pa;
  r->next = kmem.freelist;
  kmem.freelist = r;
}

// 分配物理記憶體
// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void* kalloc(void) {
  struct run* r = kmem.freelist;
  if (r)
    kmem.freelist = kmem.freelist->next;

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk

  return (void*)r;
}
