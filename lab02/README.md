# lab02 — delay() 方式 LED 控制实验

## 2/sketch_jun12a.ino — LED 闪烁

使用 `delay()` 函数控制板载 LED（GPIO 2）以 1 秒为间隔闪烁。

**核心特点**：
- `Serial.println()` 输出状态信息
- `delay(1000)` 控制亮灭时间

![实验照片1](2/afb9e71f38108919425e9e5eb496959a.jpg)
![实验照片2](2/f01690274df6ff4fbaed893a6bb66a4e.jpg)

---

## SOS_light/SOS_light.ino — SOS 摩斯电码信号灯

使用 `delay()` 函数实现 SOS 求救信号（`... --- ...`）。

**函数设计**：
- `shortBlink()` — 短闪（200ms 亮 / 200ms 灭）= `.`
- `longBlink()` — 长闪（600ms 亮 / 200ms 灭）= `-`

**时序**：
- S：短闪×3 → 字母间隔 500ms
- O：长闪×3 → 字母间隔 500ms
- S：短闪×3 → 单词间隔 2000ms

![SOS 实验视频](SOS_light/508563b3cbd44795ebdb56875c0a9881.mp4)
