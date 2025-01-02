CC = gcc
CFLAGS = -Wall -Wextra -g -I.

# 目標執行檔
TARGET = vm

# 源碼檔案
SRCS = main.c debug.c proc.c
HEADERS = types.h riscv.h

# kalloc.c 和 vm.c 已經在 main.c 中被引入，所以不需要列在 SRCS 中
OBJS = $(SRCS:.c=.o)

# 預設目標
all: $(TARGET)

# 編譯目標檔
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# 產生目標檔的規則
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理編譯產生的檔案
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) *~

# 清理編譯產生的檔案 (Windows 版)
.PHONY: wclean
wclean:
	del /f $(TARGET) $(OBJS) *~

# 執行程式
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# 除錯模式
.PHONY: debug
debug: $(TARGET)
	gdb $(TARGET)

# 檢查記憶體洩漏
.PHONY: memcheck
memcheck: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)
