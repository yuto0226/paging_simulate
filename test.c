#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "riscv.h"
#include "debug.h"
#include "memorylayout.h"

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
    pagetable_t pagetable = uvmcreate();
    if (pagetable == 0) {
        failed("uvmcreate()");
        return;
    }

    uint64 va = PGSIZE;
    uint64 pa = (uint64)kalloc();
    if (pa == 0) {
        failed("kalloc()");
        return;
    }

    if (mappages(pagetable, va, PGSIZE, pa, PTE_W) != 0) {
        failed("mappages()");
        return;
    }

    passed("mappages()");

    uvmunmap(pagetable, va, 1, 1);
    passed("uvmunmap()");
}
