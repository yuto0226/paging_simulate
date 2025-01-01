#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "riscv.h"
#include "debug.h"
#include "memorylayout.h"
#include "types.h"
#include "swap.h"

#define passed(fmt, ...) printf(\
    COLOR_GREEN "[+] " COLOR_RESET fmt COLOR_BOLD_GREEN " passed\n" COLOR_RESET,\
    ##__VA_ARGS__\
)
#define failed(fmt, ...) printf(\
    COLOR_RED "[-] " COLOR_RESET fmt COLOR_BOLD_RED " failed\n" COLOR_RESET,\
    ##__VA_ARGS__\
)

void pa_test() {
    void *p = kalloc();
    *(uint64 *)p = 0xdeadbeef;
    info("p(%p) = 0x%lx\n", p, *(uint64 *)p);
    kfree(p);
    passed("kalloc() and kfree()");
}

void va_test() {
    pagetable_t pagetable = uvmcreate();
    if (pagetable == 0) {
        failed("uvmcreate()");
        return;
    }

    uint64 oldsz = 0;
    uint64 newsz = PGSIZE * 2; // 分配兩個頁面大小的記憶體

    uint64 sz = uvmalloc(pagetable, oldsz, newsz, PTE_W);
    if (sz != newsz) {
        failed("uvmalloc()");
        return;
    }

    passed("uvmalloc()");

    sz = uvmdealloc(pagetable, newsz, oldsz);
    if (sz != oldsz) {
        failed("uvmdealloc()");
        return;
    }

    passed("uvmalloc() and uvmdealloc()");
}

void page_test() {
    uint64 *data = kalloc();
    uint64 offset = (uint64)data & 0x0fff;
    info("data=0x%lx, offset=0x%lx\n", (uint64)data, offset);

    pagetable_t pagetable = uvmcreate();
    if (pagetable == 0) {
        failed("uvmcreate()");
        return;
    }

    uint64 va = 0x1000;

    mappages(pagetable, va, PGSIZE, (uint64)data, PTE_U | PTE_R | PTE_W);
    pte_t *pte = walk(pagetable, va, 0);
    pte_info(pte);

    uint64 addr = walkaddr(pagetable, va);
    info("addr=0x%lx\n", addr);

    if(PTE2PA(*pte) != addr) {
        failed("walkaddr()");
        return;
    }

    page_out(pagetable, va);
    if ((*pte & PTE_V) != 0) {
        failed("page_out()");
        return;
    }

    pte_info(pte);

    page_in(pagetable, va);
    if ((*pte & PTE_V) == 0) {
        failed("page_in()");
        return;
    }

    pte_info(pte);

    passed("page_out() and page_in()");
}