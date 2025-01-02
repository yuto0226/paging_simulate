#include "proc.h"

// 初始化進程的函式
void init_proc(struct proc* p, char* name, pagetable_t pgtbl, uint64 sz, uint64 frame_sz) {
    p->pgtbl = pgtbl;                // 初始化頁表為空
    p->sz = sz;                   // 初始化進程大小為 0
    p->frame_sz = frame_sz;             // 初始化分配的框架大小為 0
    p->page_list = NULL;         // 初始化頁面列表為空
    strcpy(p->name, name);
}

struct page* init_page_node(uint64 va, struct page* next) {
    struct page* node = (struct page*) malloc(sizeof(struct page));
    node->va = va;
    node->next = next;
    node->count = 0;
    return node;
}

void push_page(struct page** list, uint64 va) {
    if(*list == NULL) {
        *list = init_page_node(va, NULL);
        return;
    }
    struct page* curr = *list;
    while(curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = init_page_node(va, NULL);
}

uint64 pop_page(struct page **list) {
    if(*list == NULL) return -1;
    struct page* temp = *list;
    *list = (*list)->next;
    uint64 va = temp->va;
    free(temp);
    return va;
}

struct page* find_page(struct page **list, uint64 va) {
    if(*list == NULL) {
        return NULL;
    }
    struct page* curr = *list;
    while(curr != NULL) {
        if(curr->va == va) break;
        curr = curr->next;
    }
    return curr;
}