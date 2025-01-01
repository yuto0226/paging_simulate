#ifndef VM_H
#define VM_H

#include "types.h"
#include "riscv.h"

// 函式宣告
void kvminit();
pte_t* walk(pagetable_t pagetable, uint64 va, int alloc);
uint64 walkaddr(pagetable_t pagetable, uint64 va);
int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm, int valid);
void uvmunmap(pagetable_t pagetable, uint64 va, uint64 npages, int do_free);
pagetable_t uvmcreate();
uint64 uvmdealloc(pagetable_t pagetable, uint64 oldsz, uint64 newsz);
uint64 uvmalloc(pagetable_t pagetable, uint64 oldsz, uint64 newsz, int xperm);
void page_out(pagetable_t pagetable, uint64 va);
void page_in(pagetable_t pagetable, uint64 va);

#endif // VM_H