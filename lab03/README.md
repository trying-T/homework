# lab03 — PWM LED 呼吸灯实验

## 3/3.ino — LED 呼吸灯

使用 ESP32 **PWM（脉宽调制）** 控制板载 LED（GPIO 2）实现呼吸灯效果。

## PWM 配置

| 参数 | 值 | 说明 |
|------|:--:|------|
| 频率 | 5000Hz | 超出人耳范围，无啸叫 |
| 分辨率 | 8 位 | 占空比 0-255 |
| 渐变步进 | `delay(10)` | 每步 10ms |

## 呼吸算法

```cpp
// 渐亮
for(int duty = 0; duty <= 255; duty++) {
    ledcWrite(ledPin, duty);
    delay(10);
}
// 渐暗
for(int duty = 255; duty >= 0; duty--) {
    ledcWrite(ledPin, duty);
    delay(10);
}
```

## 关键知识点

- **`ledcAttach(pin, freq, resolution)`**：新版 Arduino Core API，一步完成 PWM 配置
- **`ledcWrite(pin, duty)`**：通过引脚号直接写入占空比
- **PWM 原理**：通过快速切换高低电平实现模拟电压效果（平均电压 = 占空比 × VCC）

![呼吸灯实验视频](../2d5cf91a3176be81d3a8f587616f51a9.mp4)
