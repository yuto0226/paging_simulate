#ifndef PROC_H
#define PROC_H
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "riscv.h"
#include "vm.h"

struct page {
    uint64 va;
    uint64 count;
    struct page *next;
};

struct proc {
    pagetable_t pgtbl;
    uint64 sz;                   // Size of process memory (bytes)
    uint64 frame_sz;             // Size of frame allocated
    struct page *page_list;      // 用於 FIFO 替換的頁面列表
    char name[16];
};

// 初始化進程的函式
void init_proc(struct proc* p, char* name, pagetable_t pgtbl, uint64 sz, uint64 frame_sz);
struct page *init_page_node(uint64 va, struct page *next);
void push_page(struct page** list, uint64 va);
uint64 pop_page(struct page** list);
struct page* find_page(struct page** list, uint64 va);
uint64 pop_lru_page(struct page** list);

#endif
