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

## lab01 — ESP32 Arduino 编程入门

### 1.ino — Hello ESP32

使用 Arduino 框架控制 ESP32 板载 LED（GPIO 2）以 1 秒为间隔闪烁，并通过串口输出 "Hello ESP32!"。

- `setup()` — 初始化串口（波特率 115200），设置 LED 引脚为输出模式
- `loop()` — 循环控制 LED 亮灭，每次延时 1 秒

---

## lab02 — delay() 方式 LED 控制实验

### 2/sketch_jun12a.ino — LED 闪烁（delay 方式）

使用 `delay()` 函数控制 ESP32 板载 LED（GPIO 2）以 1 秒为间隔闪烁，并通过串口输出状态信息。

- `setup()` — 初始化串口（波特率 115200），设置 LED 引脚为输出模式
- `loop()` — 循环控制 LED 亮灭，每次切换后 `delay(1000)`，并通过 `Serial.println()` 输出 "LED ON" / "LED OFF"

![实验照片](lab02/2/afb9e71f38108919425e9e5eb496959a.jpg)
![实验照片](lab02/2/f01690274df6ff4fbaed893a6bb66a4e.jpg)

### SOS_light/SOS_light.ino — SOS 摩斯电码信号灯（delay 方式）

使用 `delay()` 函数控制 ESP32 板载 LED 发送 SOS 求救信号（摩斯电码：`... --- ...`）。

- `shortBlink()` — 短闪烁（200ms 亮 / 200ms 灭），表示摩斯电码的 `.`
- `longBlink()` — 长闪烁（600ms 亮 / 200ms 灭），表示摩斯电码的 `-`
- `loop()` — 循环发送 S（短闪×3）→ O（长闪×3）→ S（短闪×3），字母间隔 500ms，单词间隔 2000ms

![SOS实验视频](lab02/SOS_light/508563b3cbd44795ebdb56875c0a9881.mp4)
