#include "types.h"
#include "riscv.h"
#include "vm.h"
#include "proc.h"
#include "debug.h"

enum {
    FIFO,
    LRU,
};

// 輸入 va 回傳 pa
// page fault 會用 fifo page replacement algorithm
uint64 mmu(struct proc *p, uint64 va, int algo) {
    if(va >= p->sz * 0x1000) {
        error(COLOR_BOLD_RED"錯誤的記憶體存取\n"COLOR_RESET);
        return 0;
    }

    uint64 offset = va & 0xfff;
    uint64 pa;
    pte_t* pte = walk(p->pgtbl, va, 0);
    
    // page fault
    if ((*pte & PTE_V) == 0 || (*pte & PTE_S) != 0) {
        printf("\n");
        warn(COLOR_BOLD_YELLOW"PAGE FAULT\n\n"COLOR_RESET);
        
        // 需要替換頁面
        uint64 victim_page;
        if(algo == FIFO) victim_page = pop_page(&p->page_list); // 獲取 FIFO 隊列的第一個頁面
        else if(algo == LRU) {
            victim_page = pop_lru_page(&p->page_list);
            if(victim_page == MAXVA + 1) panic("find lru page");
        } else {
            panic("no page replacement algorithm.");
        }
        info("mmu: select victim page= 0x%08lx\n", victim_page);

        // 使用 page_out 將舊頁面移出
        info("page_out: va=0x%08lx\n", victim_page);
        page_out(p->pgtbl, victim_page);
        ok("page out successful.\n");

        info("page in: page virtual addr= 0x%08lx\n", PGROUNDDOWN(va));
        page_in(p->pgtbl, PGROUNDDOWN(va));
        push_page(&p->page_list, PGROUNDDOWN(va));
        ok("page in: successful.\n");
        page_list_info(&p->page_list);
    } else {
        pa = PTE2PA(*pte) | offset;
    }
    info("update count in list\n");
    struct page* temp = find_page(&p->page_list, PGROUNDDOWN(va));
    if(temp) temp->count++;
    else {
        warn("更新參考次數失敗\n");
    }
    pte_info(pte);
    page_list_info(&p->page_list);

    return pa;
}