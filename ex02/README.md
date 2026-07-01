# ex02 — millis() 函数 LED 1Hz 闪烁

## 功能说明

使用 `millis()` 函数替代 `delay()` 实现**非阻塞式** LED 1Hz 闪烁。

## 关键知识点

- **`millis()`**：获取 ESP32 系统启动后的毫秒数
- **非阻塞编程**：不使用 `delay()`，避免阻塞其他代码执行
- **1Hz 闪烁**：周期 1000ms，亮 500ms / 灭 500ms
- **`unsigned long` 溢出安全**：`currentMillis - previousMillis` 自动处理溢出

## 核心代码

```cpp
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
```

## 对比 delay() 方式

| | delay() 方式 | millis() 方式 |
|------|:--:|:--:|
| 代码复杂度 | 简单 | 稍复杂 |
| 是否阻塞 | 是 | 否 |
| 多任务能力 | 差 | 强 |
| 精确度 | 受 loop() 执行时间影响 | 更精确 |

## 实验照片

![照片1](dc2e2bcaf62d70147c179d33d154f716.jpg)
![照片2](e42bcdc1e4666338a6d8953225e269d6.jpg)
