#include "debug.h"

void
panic(char* s) {
  printf(COLOR_BOLD_RED "!![PANIC]!! %s\n" COLOR_RESET, s);
  for(;;)
    ;
}
