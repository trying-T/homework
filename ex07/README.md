# ex07 — Web 网页端无极调光器

## 功能说明

通过手机/电脑浏览器访问 ESP32 网页，使用**滑动条**实时控制 LED 亮度（0-255 无极调节）。

## 技术栈

| 技术 | 用途 |
|------|------|
| WiFi AP 模式 | ESP32 自建热点 `ESP32-DIMMER` |
| WebServer | HTTP 服务，端口 80 |
| AJAX (Fetch API) | 网页实时发送亮度值 |
| PWM | LED 无极调光 |

## 网络信息

| 项目 | 值 |
|------|------|
| SSID | `ESP32-DIMMER` |
| 密码 | `12345678` |
| IP | `192.168.4.1` |

## 关键知识点

- **WiFi.softAP()**：ESP32 创建 WiFi 热点
- **WebServer**：处理 HTTP 路由（`/`、`/set`、`/state`）
- **前端 AJAX**：滑动条 `input` 事件触发 `fetch("/set?value=...")` 实时通信
- **Arduino Core 兼容**：兼容 2.x / 3.x PWM API
- **raw string literal**：`R"rawliteral(...)rawliteral"` 嵌入完整 HTML

## 使用步骤

1. 上传代码到 ESP32
2. 手机/电脑连接 `ESP32-DIMMER` 热点（密码 `12345678`）
3. 浏览器访问 `192.168.4.1`
4. 拖动滑动条实时控制 LED 亮度
