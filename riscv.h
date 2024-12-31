#ifndef RISCV_H
#define RISCV_H

#include "types.h"

typedef uint64 pte_t;
typedef uint64* pagetable_t; // 512 PTEs

#define PGSIZE 4096 // bytes per page
#define PGSHIFT 12  // bits of offset within a page

// 將大小向上取整到最接近的頁面大小
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
// 將地址向下取整到最接近的頁面邊界
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

#define PTE_V (1L << 0) // valid
#define PTE_R (1L << 1) // readable
#define PTE_W (1L << 2) // writable
#define PTE_X (1L << 3) // executable
#define PTE_U (1L << 4) // user can access
#define PTE_G (1L << 5) // global
#define PTE_A (1L << 6) // accessed
#define PTE_D (1L << 7) // dirty, modified
#define PTE_S (1L << 8) // swapped

// shift a physical address to the right place for a PTE.
#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)

// extract the physical address from a PTE.
#define PTE2PA(pte) (((pte) >> 10) << 12)

// extract the flags from a PTE.
#define PTE_FLAGS(pte) ((pte) & 0x3FF)

// extract the three 9-bit page table indices from a virtual address.
#define PXMASK          0x1FF // 9 bits
// shift the virtual address to the right place for a page table index.
#define PXSHIFT(level)  (PGSHIFT+(9*(level)))
// get the page table index from a virtual address.
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)

// one beyond the highest possible virtual address.
// MAXVA is actually one bit less than the max allowed by
// Sv39, to avoid having to sign-extend virtual addresses
// that have the high bit set.
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))

#endif
