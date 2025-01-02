// 直接引入 main.c 然後呼叫測試函式使用即可
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "riscv.h"
#include "debug.h"
#include "memorylayout.h"
#include "types.h"
#include "swap.h"
#include "vm.h"

#define passed(fmt, ...) printf(\
    COLOR_GREEN "[+] " COLOR_RESET fmt COLOR_BOLD_GREEN " passed\n" COLOR_RESET,\
    ##__VA_ARGS__\
)
#define failed(fmt, ...) printf(\
    COLOR_RED "[-] " COLOR_RESET fmt COLOR_BOLD_RED " failed\n" COLOR_RESET,\
    ##__VA_ARGS__\
)

void pa_test() {
    printf("\n##################################\n");
    printf("#      physica address test      #\n");
    printf("##################################\n");
    printf("測試實體憶體的分配\n\n");
    void *p = kalloc();
    *(uint64 *)p = 0xdeadbeef;
    info("p(%p) = 0x%lx\n", p, *(uint64 *)p);
    kfree(p);
    passed("kalloc() and kfree()");
}

void va_test() {
    printf("\n##################################\n");
    printf("#      virtual address test      #\n");
    printf("##################################\n");
    printf("測試虛擬記憶體的建立與分配\n\n");

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
    printf("\n##################################\n");
    printf("#           page test            #\n");
    printf("##################################\n");
    printf("測試 1 個 frame 的情況下重複 page in, page out.\n\n");

    swap_init();
    pagetable_t pagetable = uvmcreate();

    // create page 0
    uint64 *data = kalloc();
    memset(data, 'A', PGSIZE);
    strcpy((char*)data, "This data from section A.\n");
    info("data=0x%lx\n", (uint64)data);

    uint64 va = 0x1000;

    mappages(pagetable, va, PGSIZE, (uint64)data, PTE_U | PTE_R | PTE_W);
    pte_t *pte = walk(pagetable, va, 0);
    pte_info(pte);

    // create page 1 with swap space
    uint64* data1 = kalloc();
    memset(data1, 'B', PGSIZE);
    strcpy((char*)data1, "This data from section B.\n");
    info("data1=0x%lx\n", (uint64) data1);

    uint64 va1 = 0x2000;
    int si = swap_out(data1);
    mappages(pagetable, va1, PGSIZE, si, PTE_U | PTE_R | PTE_W | PTE_S);
    pte_t* pte1 = walk(pagetable, va1, 0);
    pte_info(pte1);

    // page out page 0
    printf("\n");
    warn(COLOR_BOLD_YELLOW"PAGE OUT\n\n"COLOR_RESET);
    page_out(pagetable, va);
    if((*pte & PTE_S) == 0) {
        failed("page_out()");
        return;
    } else {
        passed("page_out()");
    }

    pte_info(pte);

    // page in page 1
    printf("\n");
    warn(COLOR_BOLD_YELLOW"PAGE IN\n\n"COLOR_RESET);
    page_in(pagetable, va1);
    if ((*pte1 & PTE_S) != 0) {
        failed("page_in()");
        return;
    } else {
        passed("page_in()");
    }

    pte_info(pte1);
    printf("page 2 data: %s\n", (char*)PTE2PA(*pte1));

    // page  out page 1
    printf("\n");
    warn(COLOR_BOLD_YELLOW"PAGE OUT\n\n"COLOR_RESET);
    page_out(pagetable, va1);
    if((*pte1 & PTE_S) == 0) {
        failed("page_out()");
        return;
    } else {
        passed("page_out()");
    }

    pte_info(pte1);

    // swap in page 0
    printf("\n");
    warn(COLOR_BOLD_YELLOW"PAGE OUT\n\n"COLOR_RESET);
    page_in(pagetable, va);
    if((*pte & PTE_S) != 0) {
        failed("page_in()");
        return;
    } else {
        passed("page_in()");
    }

    pte_info(pte);
    printf("page 2 data: %s\n", (char*) PTE2PA(*pte));

    passed("page_out() and page_in()");
}