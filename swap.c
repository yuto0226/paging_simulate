#include "types.h"
#include "riscv.h"
#include "debug.h"
#include "memorylayout.h"
#include "swap.h"

char swap_space[SWAP_SIZE * PGSIZE];  // 模擬磁碟的交換區域
int swap_map[SWAP_SIZE];  // 交換區域的使用狀態

void swap_init() {
  memset(swap_map, 0, sizeof(swap_map));
}

// 將 frame 交換到交換區域
int swap_out(void *pa) {
  for (int i = 0; i < SWAP_SIZE; i++) {
    if (swap_map[i] == 0) {
      memcpy(&swap_space[i * PGSIZE], pa, PGSIZE);
      swap_map[i] = 1;
      return i;  // 返回交換區域的索引
    }
  }
  panic("swap_out: no space in swap area");
  return -1;
}

// 將交換區域中的頁面交換到物理地址
void swap_in(int index, void *pa) {
  if (index < 0 || index >= SWAP_SIZE || swap_map[index] == 0) {
    panic("swap_in: invalid swap index");
  }
  memcpy(pa, &swap_space[index * PGSIZE], PGSIZE);
  swap_map[index] = 0;
}
