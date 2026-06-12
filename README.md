# 作业汇总

## ex01 — C 语言基础

### hello.c — 基础版
最简单的 Hello World 程序，直接输出 "Hello, World!"。

### input.c — 函数版
在 Hello World 基础上增加了函数封装：

- `getInput()` — 获取用户键盘输入
- `showOutput()` — 显示输出用户输入的内容
- `main()` — 调用上述两个函数完成输入输出

---

## lab01 — ESP32 Arduino 编程

### sketch_jun12a.ino — LED 闪烁

使用 Arduino 框架控制 ESP32 板载 LED（GPIO 2）以 1 秒为间隔闪烁，并通过串口输出状态信息。

- `setup()` — 初始化串口（波特率 115200），设置 LED 引脚为输出模式
- `loop()` — 循环控制 LED 亮灭，每次切换后延时 1 秒，并通过 `Serial.println()` 输出 "LED ON" / "LED OFF"
