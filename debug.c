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
  printf("--------------------------------\n");
  printf("frame addr      : 0x%lx\n", PTE2PA(*pte));
  printf("Valid           : %s\n", (*pte & PTE_V) ? "Yes" : "No");
  printf("Readable        : %s\n", (*pte & PTE_R) ? "Yes" : "No");
  printf("Writable        : %s\n", (*pte & PTE_W) ? "Yes" : "No");
  printf("Executable      : %s\n", (*pte & PTE_X) ? "Yes" : "No");
  printf("User Accessible : %s\n", (*pte & PTE_U) ? "Yes" : "No");
  printf("Global          : %s\n", (*pte & PTE_G) ? "Yes" : "No");
  printf("Accessed        : %s\n", (*pte & PTE_A) ? "Yes" : "No");
  printf("Dirty           : %s\n", (*pte & PTE_D) ? "Yes" : "No");
  printf("Swapped         : %s\n", (*pte & PTE_S) ? "Yes" : "No");
  printf("--------------------------------\n");
}
