# ex09 — 实时传感器 Web 仪表盘

## 功能说明

构建实时传感器数据监控仪表盘：
- 🌐 网页通过 **AJAX** 每 200ms 轮询 ESP32 触摸传感器数据
- 📊 实时数值显示 + 进度条可视化
- 🔄 `touchRead()` 值越小表示触摸越明显

## 硬件

| 引脚 | 功能 |
|------|------|
| GPIO4 | 触摸传感器 T0 |
| — | 无 LED（纯数据采集展示） |

## 技术栈

| 技术 | 用途 |
|------|------|
| WiFi AP | ESP32 自建 `ESP32-DASHBOARD` 热点 |
| WebServer | HTTP 路由：`/`、`/touch`、`/touchJson` |
| AJAX (`setInterval`) | 前端每 200ms 请求 `/touch` 接口 |
| Fetch API | JavaScript 异步数据请求 |
| CSS 进度条 | 触摸值可视化 |

## 网络信息

| 项目 | 值 |
|------|------|
| SSID | `ESP32-DASHBOARD` |
| 密码 | `12345678` |
| IP | `192.168.4.1` |

## 关键知识点

- **实时数据上报**：ESP32 不仅能下发网页，还能上报传感器数据
- **AJAX 轮询**：`setInterval(updateTouchValue, 200)` 定时拉取数据
- **JSON API**：提供 `/touch`（纯文本）和 `/touchJson`（JSON）两种接口
- **进度条映射**：`touchValue / maxValue * 100%`

## 使用步骤

1. 上传代码到 ESP32
2. 连接 `ESP32-DASHBOARD` 热点
3. 浏览器访问 `192.168.4.1`
4. 手指触摸/靠近 GPIO4，观察数值下降和进度条变化
