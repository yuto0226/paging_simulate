#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "riscv.h"
#include "kalloc.c"
#include "vm.c"
#include "debug.h"
#include "memorylayout.h"
#include "test.c"

char end[PA_SIZE];

int main(int argc, char *argv[]) {
  (void)argc; (void)argv;

  // 初始化物理和虛擬記憶體
    kinit();
    ok("physical memory initialized\n");
    kvminit();
    ok("virtual memory initialized\n");

    // 執行測試
    pa_test();
    va_test();
    page_test();

  return 0;
}
