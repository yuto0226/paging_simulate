#ifndef MEMORYLAYOUT_H
#define MEMORYLAYOUT_H

#include "riscv.h"

// 定義物理記憶體大小
#define PA_SIZE (PGSIZE * 64)
// 定義物理記憶體結束位址
#define PHYSTOP ((uint64)end + PA_SIZE)

extern char end[];

#endif
