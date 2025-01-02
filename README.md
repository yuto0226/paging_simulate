# xv6 paging simulate

大同大學作業系統期末專題，實作 paging 和 page replacement。大量挪用了 xv6-riscv 的程式。

## 安裝與使用

推薦使用 linux 系統操作。

複製此專案：
```bash
git clone https://github.com/yuto0226/paging_simulate.git
```
進入專案目錄：
```bash
cd paging_simulate
```
編譯專案：
```bash
make run
```

## 重點概念

- 使用額外的 `PTE_S` 在分頁表中紀錄是否被換出到交換區
- 維護一個 `swap_map` 紀錄交換區的使用狀況
- 預設行程會維護一個 page table 和負責記錄可用分頁的 page list
- 利用程式模擬 MMU 並實作 page fault 的分頁替換演算法

![](img/flow_chart.png)


