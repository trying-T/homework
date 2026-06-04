#include <stdio.h>

// 获取用户输入
void getInput(char *buffer, int size) {
    printf("请输入内容：");
    fgets(buffer, size, stdin);
}

// 显示输出内容
void showOutput(const char *content) {
    printf("你输入的内容是：%s", content);
}

int main() {
    char name[100];

    getInput(name, sizeof(name));
    showOutput(name);

    return 0;
}
