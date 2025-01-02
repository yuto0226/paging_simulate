#include "debug.h"
#include "proc.h"
#include "riscv.h"

void
panic(char* s) {
  printf(COLOR_BOLD_RED "!![PANIC]!! %s\n" COLOR_RESET, s);
  for(;;)
    ;
}

void pte_info(pte_t *pte) {
  info("pte(0x%016lx) info: 0x%010lx\n", (uint64)pte, *pte);
  if(PTE_FLAGS(*pte) & PTE_S) {
    info("this page is swapped\n");
  }
  printf("--------------------------------\n");
  printf("       addr        flags\n");
  printf("+----------------+-------+\n");
  if(PTE_FLAGS(*pte) & PTE_S)
    printf("| 0x%012lx | 0x%03lx |\n", *pte >> 10,  PTE_FLAGS(*pte));
  else
    printf("| 0x%012lx | 0x%03lx |\n", PTE2PA(*pte), PTE_FLAGS(*pte));
  printf("+----------------+-------+\n\n");
  if(PTE_FLAGS(*pte) & PTE_S)
    printf("swap index      : 0x%lx\n", *pte >> 10);
  else
    printf("frame addr      : 0x%lx\n", PTE2PA(*pte));
  printf("Valid           : %s\n", (*pte & PTE_V) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("Readable        : %s\n", (*pte & PTE_R) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("Writable        : %s\n", (*pte & PTE_W) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("Executable      : %s\n", (*pte & PTE_X) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("User Accessible : %s\n", (*pte & PTE_U) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("Global          : %s\n", (*pte & PTE_G) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("Accessed        : %s\n", (*pte & PTE_A) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("Dirty           : %s\n", (*pte & PTE_D) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("Swapped         : %s\n", (*pte & PTE_S) ? COLOR_BOLD_GREEN"Yes"COLOR_RESET : "No");
  printf("--------------------------------\n\n");
}

void pgtbl_info(pagetable_t pgtbl, uint64 sz) {
  info("page table(size=%ld):\n", sz);
  for(uint64 i = 0; i < 256; i++) {  // 從level 2開始，level 0結束
    pte_t *pte = &pgtbl[i];
    if((*pte & PTE_V) != 0) {  // 檢查頁表條目是否有效
      printf("pgtbl[%ld]:\n", i);
      pte_info(pte);  // 顯示該頁表條目的詳細資訊
    }
  }
}

void page_list_info(struct page** list) {
  info("page list(0x%lx):\n", (uint64)*list);
  struct page* curr = *list;
  while(curr != NULL) {
    printf("\\___virtual addr= 0x%04lx\n", curr->va);
    curr = curr->next;
  }
  printf("\n");
}
