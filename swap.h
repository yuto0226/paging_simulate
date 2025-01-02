#ifndef SWAP_H
#define SWAP_H

#define SWAP_SIZE 16  // 定義交換區域大小（頁數）

void swap_init();
uint64 swap_out(void *pa);
void swap_in(int index, void *pa);

#endif  // SWAP_H