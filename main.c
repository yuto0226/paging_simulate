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

int main(int argc, char *argv[]) {
  (void)argc; (void)argv;

  // 初始化物理和虛擬記憶體
  kinit();
  ok("physical memory initialized\n");

  // 初始化交換區域
  swap_init();
  ok("swap area initialized\n");

  pagetable_t pgtbl = uvmcreate();

  // process 維護的資訊
  struct proc pcb;
  init_proc(&pcb, "test", pgtbl, 7, 3);

  exec(&pcb);
  page_list_info(&pcb.page_list);

  // 使用 mmu 轉換虛擬地址
  uint64 va = 0x1000;
  for(;;) {
    printf(COLOR_BOLD_RED"$ "COLOR_RESET);
    scanf("%lx", &va);
    uint64 pa = mmu(&pcb, va); // 轉換虛擬地址到物理地址
    printf("va: 0x%016lx -> pa: 0x%016lx\n", va, pa);
  }
  
  return 0;
}

void exec(struct proc *pcb) {
  info("map user memory space\n");
  for(uint64 page_count = 0; page_count < pcb->sz; page_count++) {
    uint64 va = page_count * 0x1000;
    uint64* data = kalloc();
    if(data == 0) { // 檢查 kalloc() 是否成功
      error("記憶體分配失敗");
      panic("main: kalloc()");
    }

    if(page_count < pcb->frame_sz) {
      info("map: va=0x%04lx --> pa=0x%lx\n", va, (uint64) data);
      mappages(pcb->pgtbl, va, PGSIZE, (uint64) data, PTE_U);
      push_page(&pcb->page_list, va);
    } else {
      uint64 si = swap_out(data);
      info("va=0x%04lx swapped into index=0x%lx\n", page_count * 0x1000, si);
      mappages(pcb->pgtbl, va, PGSIZE, si, PTE_U | PTE_S);
    }
  }
  ok("map user memory space complete\n");
}
