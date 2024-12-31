#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include "types.h"
#include "riscv.h"

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"

#define COLOR_BOLD_RED "\033[1;31m"
#define COLOR_BOLD_GREEN "\033[1;32m"
#define COLOR_BOLD_YELLOW "\033[1;33m"
#define COLOR_BOLD_BLUE "\033[1;34m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_BOLD_CYAN "\033[1;36m"

#define    ok(fmt, ...) printf(COLOR_GREEN "[+] " COLOR_RESET fmt, ##__VA_ARGS__)
#define  info(fmt, ...) printf(COLOR_BLUE  "[*] " COLOR_RESET fmt, ##__VA_ARGS__)
#define  warn(fmt, ...) printf(COLOR_YELLOW "[!] " COLOR_RESET fmt, ##__VA_ARGS__)
#define error(fmt, ...) printf(COLOR_RED    "[-] " COLOR_RESET fmt, ##__VA_ARGS__)

void panic(char *s);
void pte_info(pte_t *pte);

#endif
