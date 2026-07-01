# ex04 — ESP32 触摸自锁开关

## 功能说明

利用 ESP32 **电容触摸传感器（T0 / GPIO4）** 实现触摸自锁开关：
- 摸一下 → LED 亮并保持
- 再摸一下 → LED 灭并保持

## 硬件

| 引脚 | 功能 |
|------|------|
| GPIO4 | 触摸传感器 T0 |
| GPIO2 | 板载 LED |

## 关键技术

- **`touchRead(pin)`**：读取电容触摸值，触摸时数值下降
- **触摸阈值（threshold）**：触摸判定阈值，需通过串口监视器调校
- **软件防抖（debounce）**：80ms 防抖延迟，滤除触摸抖动
- **边缘检测**：只在"未触摸 → 触摸"的瞬间翻转 LED 状态

## 核心逻辑

```cpp
// 只在下降沿触发（释放→触摸）
if (previousStableState == false && stableTouchState == true) {
    ledState = !ledState;  // 翻转 LED
}
```

## 调试

串口监视器输出触摸值、原始状态、LED 状态，方便调整 `threshold`。
