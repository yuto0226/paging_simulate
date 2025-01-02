#include "debug.h"
#include "riscv.h"

void
panic(char* s) {
  printf(COLOR_BOLD_RED "!![PANIC]!! %s\n" COLOR_RESET, s);
  for(;;)
    ;
}

void pte_info(pte_t *pte) {
  info("pte info: 0x%lx\n", *pte);
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
  printf("+----------------+-------+\n");
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
  printf("--------------------------------\n");
}
