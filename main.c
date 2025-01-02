#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "riscv.h"
#include "kalloc.c"
#include "vm.c"
#include "debug.h"
#include "memorylayout.h"
#include "test.c"
#include "swap.c"
#include "proc.h"
#include "mmu.c"

char end[PA_SIZE];
void exec(struct proc *pcb);

int main() {
  int page_sz, frame_sz, algo;
  info("輸入分頁數量: ");
  scanf("%d", &page_sz);
  info("輸入分欄數量(<128): ");
  scanf("%d", &frame_sz);
  info("選擇替換演算法(1.fifo, 2.lru):");
  scanf("%d", &algo);

  // 初始化物理和虛擬記憶體
  kinit();
  ok("物理記憶體模擬初始化完成\n");

  // 初始化交換區域
  swap_init();
  ok("交換區域模擬初始化完成\n");

  pagetable_t pgtbl = uvmcreate();

  // process 維護的資訊
  struct proc pcb;
  init_proc(&pcb, "test", pgtbl, page_sz, frame_sz);

  exec(&pcb);
  page_list_info(&pcb.page_list);

  // 使用 mmu 轉換虛擬地址
  uint64 va = 0x1000;
  for(;;) {
    info("輸入虛擬記憶體位址:\n");
    printf(COLOR_BOLD_RED"$ "COLOR_RESET);
    scanf("%lx", &va);
    uint64 pa = mmu(&pcb, va); // 轉換虛擬地址到物理地址
    printf("virtual addr= 0x%08lx -> physical addr= 0x%016lx\n", va, pa);
  }
  
  return 0;
}

void exec(struct proc *pcb) {
  info("映射使用者記憶體空間...\n");
  for(uint64 page_count = 0; page_count < pcb->sz; page_count++) {
    uint64 va = page_count * 0x1000;
    uint64* data = kalloc();
    if(data == 0) { // 檢查 kalloc() 是否成功
      error("記憶體分配失敗");
      panic("main: kalloc()");
    }

    if(page_count < pcb->frame_sz) {
      info("map : virtual addr= 0x%04lx --> physical addr= 0x%lx\n", va, (uint64) data);
      mappages(pcb->pgtbl, va, PGSIZE, (uint64) data, PTE_U);
      push_page(&pcb->page_list, va);
    } else {
      uint64 si = swap_out(data);
      info("swap: virtual addr= 0x%04lx --> swap index= 0x%lx\n", page_count * 0x1000, si);
      mappages(pcb->pgtbl, va, PGSIZE, si, PTE_U | PTE_S);
    }
    // pte_t *pte = walk(pcb->pgtbl, va, 0);
    // pte_info(pte);
  }
  ok("映射使用者記憶體空間完成\n");
}
