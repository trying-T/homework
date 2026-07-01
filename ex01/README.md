# ex01 — C 语言基础

## hello.c

最简单的 Hello World 程序，直接输出 "Hello, World!"。

```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```

## input.c

在 Hello World 基础上增加了函数封装：

- `getInput()` — 获取用户键盘输入
- `showOutput()` — 显示输出用户输入的内容
- `main()` — 调用上述两个函数完成输入输出

```c
void getInput(char *buffer, int size) {
    printf("请输入内容：");
    fgets(buffer, size, stdin);
}

void showOutput(const char *content) {
    printf("你输入的内容是：%s", content);
}
```
