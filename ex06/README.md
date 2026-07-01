# ex06 — 警车双闪灯效（双通道 PWM）

## 功能说明

利用两个 LED 实现**反相渐变**的警车双闪效果：
- LED A 逐渐变亮时，LED B 逐渐变暗
- 视觉上呈现交替闪烁的警车灯效

## 硬件

| 引脚 | 功能 |
|------|------|
| GPIO18 | LED A（PWM 通道 0） |
| GPIO19 | LED B（PWM 通道 1） |

## PWM 配置

| 参数 | 值 |
|------|:--:|
| 频率 | 5000Hz |
| 分辨率 | 8 位（0-255） |

## 关键技术

- **双通道 PWM**：独立控制两个 LED 亮度
- **反相输出**：`dutyA = x`；`dutyB = 255 - x`
- **Arduino Core 兼容**：兼容 2.x（`ledcSetup`/`ledcWrite`）和 3.x（`ledcAttachChannel`/`ledcWriteChannel`）

## 反相逻辑

```cpp
for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(LED_A, duty);        // A 渐亮
    ledcWrite(LED_B, 255 - duty);  // B 渐暗
}
```
