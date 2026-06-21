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

---

## lab03 — PWM LED 呼吸灯实验

### 3/3.ino — LED 呼吸灯

使用 ESP32 的 `ledcAttach()` / `ledcWrite()` PWM（脉宽调制）功能控制板载 LED（GPIO 2）实现呼吸灯效果。

- `setup()` — 初始化串口，使用 `ledcAttach()` 绑定 GPIO 2，设置 PWM 频率 5000Hz、8 位分辨率（0-255）
- `loop()` — 通过 `ledcWrite()` 逐步调节占空比，实现 LED 渐亮→渐暗→渐亮的呼吸效果，每次循环后串口输出 "Breathing cycle completed"

![呼吸灯实验视频](lab03/2d5cf91a3176be81d3a8f587616f51a9.mp4)

---

## ex02 — millis() 函数 LED 1Hz 闪烁

### sketch_jun21a.ino — 非阻塞式 LED 1Hz 闪烁

使用 `millis()` 函数替代 `delay()` 实现非阻塞式 LED 1Hz 闪烁（亮 500ms / 灭 500ms）。

- `setup()` — 设置 LED 引脚（GPIO 2）为输出模式
- `loop()` — 通过 `millis()` 获取系统运行时间，每 500ms 翻转一次 LED 状态，实现 1Hz 精确闪烁，不会阻塞其他代码执行

```cpp
unsigned long previousMillis = 0;
const unsigned long interval = 500;  // 1Hz = 500ms on, 500ms off
bool ledState = LOW;

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
```

![实验照片1](ex02/dc2e2bcaf62d70147c179d33d154f716.jpg)
![实验照片2](ex02/e42bcdc1e4666338a6d8953225e269d6.jpg)
