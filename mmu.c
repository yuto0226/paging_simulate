#include "types.h"
#include "riscv.h"
#include "vm.h"
#include "proc.h"
#include "debug.h"

// 輸入 va 回傳 pa
// page fault 會用 fifo page replacement algorithm
uint64 mmu(struct proc *p, uint64 va) {
    if(va > p->sz * 0x1000) {
        error(COLOR_BOLD_RED"錯誤的記憶體存取\n"COLOR_RESET);
        return 0;
    }

    uint64 offset = va & 0xfff;
    uint64 pa;
    pte_t* pte = walk(p->pgtbl, va, 0);
    
    // page fault
    if ((*pte & PTE_V) == 0 || (*pte & PTE_S) != 0) {
        printf("\n");
        warn("PAGE FAULT\n\n");
        // 需要替換頁面
        uint64 victim_page = pop_page(&p->page_list); // 獲取 FIFO 隊列的第一個頁面
        info("mmu: select victim 0x%lx\n", victim_page);

        // 使用 page_out 將舊頁面移出
        page_out(p->pgtbl, victim_page);
        ok("page out successful.\n");

        page_in(p->pgtbl, PGROUNDDOWN(va));
        push_page(&p->page_list, PGROUNDDOWN(va));
        ok("page in successful.\n");
        page_list_info(&p->page_list);
    } else {
        pa = PTE2PA(*pte) | offset;
    }

    return pa;
}