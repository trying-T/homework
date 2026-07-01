// ex09：实时传感器Web仪表盘
// 功能：ESP32网页实时显示触摸传感器 touchRead() 数值
// 重点：不仅能下发网页，还能实时上报传感器数据

#include <WiFi.h>
#include <WebServer.h>

// ================= WiFi AP 配置 =================
// ESP32 创建自己的热点
const char* ap_ssid = "ESP32-DASHBOARD";
const char* ap_pass = "12345678";   // 至少8位

WebServer server(80);

// ================= 触摸引脚配置 =================
// ESP32 T0 触摸通道对应 GPIO4
#define TOUCH_PIN 4

// 保存当前触摸值
int currentTouchValue = 0;

// ================= 生成网页 =================
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 实时传感器仪表盘</title>

  <style>
    body {
      margin: 0;
      padding: 0;
      min-height: 100vh;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Arial, sans-serif;
      background: linear-gradient(135deg, #eef2ff, #f8fafc);
      display: flex;
      justify-content: center;
      align-items: center;
      color: #111827;
    }

    .card {
      width: 88%;
      max-width: 440px;
      background: white;
      border-radius: 26px;
      padding: 30px;
      text-align: center;
      box-shadow: 0 18px 45px rgba(0, 0, 0, 0.12);
    }

    h1 {
      margin-top: 0;
      font-size: 26px;
      color: #1e3a8a;
    }

    .subtitle {
      font-size: 15px;
      color: #6b7280;
      margin-bottom: 28px;
      line-height: 1.6;
    }

    .value-label {
      color: #6b7280;
      font-size: 15px;
      margin-bottom: 10px;
    }

    .value {
      font-size: 68px;
      font-weight: 800;
      color: #2563eb;
      margin: 12px 0;
      letter-spacing: 2px;
    }

    .desc {
      color: #374151;
      font-size: 15px;
      line-height: 1.8;
      margin-top: 18px;
    }

    .bar-box {
      width: 100%;
      height: 16px;
      background: #e5e7eb;
      border-radius: 999px;
      overflow: hidden;
      margin-top: 26px;
    }

    .bar {
      height: 100%;
      width: 0%;
      background: #2563eb;
      border-radius: 999px;
      transition: width 0.15s linear;
    }

    .status {
      margin-top: 20px;
      font-size: 14px;
      color: #9ca3af;
    }

    .tip {
      margin-top: 26px;
      color: #9ca3af;
      font-size: 13px;
      line-height: 1.6;
    }
  </style>
</head>

<body>
  <div class="card">
    <h1>ESP32 实时传感器仪表盘</h1>
    <div class="subtitle">
      网页端通过 AJAX 实时读取触摸传感器数值
    </div>

    <div class="value-label">当前 Touch Value</div>
    <div class="value" id="touchValue">--</div>

    <div class="bar-box">
      <div class="bar" id="touchBar"></div>
    </div>

    <div class="desc">
      手指逐渐靠近或触摸 GPIO4 引脚时，数值会明显变小；<br>
      手指离开后，数值会恢复。
    </div>

    <div class="status" id="statusText">正在连接 ESP32...</div>

    <div class="tip">
      手机或电脑连接 ESP32-DASHBOARD 热点后，访问 192.168.4.1
    </div>
  </div>

  <script>
    const valueText = document.getElementById("touchValue");
    const statusText = document.getElementById("statusText");
    const touchBar = document.getElementById("touchBar");

    function updateTouchValue() {
      fetch("/touch")
        .then(response => response.text())
        .then(data => {
          let value = parseInt(data);

          if (!isNaN(value)) {
            valueText.innerText = value;
            statusText.innerText = "数据更新中";

            /*
              这里做一个简单的进度条显示：
              你的触摸值大概可能在 200~1200 之间变化。
              数值越小，说明触摸越明显。
            */
            let maxValue = 1300;
            let minValue = 0;

            let percent = value / maxValue * 100;

            if (percent > 100) percent = 100;
            if (percent < 0) percent = 0;

            touchBar.style.width = percent + "%";
          }
        })
        .catch(error => {
          statusText.innerText = "连接失败，正在重试...";
          console.log(error);
        });
    }

    // 每 200ms 请求一次 ESP32 的触摸传感器数据
    setInterval(updateTouchValue, 200);

    // 页面打开后立即执行一次
    updateTouchValue();
  </script>
</body>
</html>
)rawliteral";

  return html;
}

// ================= Web 路由处理 =================

// 主页：返回 HTML 页面
void handleRoot() {
  server.send(200, "text/html", makePage());
}

// 实时触摸数据接口：网页 AJAX 会不断请求这个地址
void handleTouchValue() {
  currentTouchValue = touchRead(TOUCH_PIN);

  Serial.print("Touch Value: ");
  Serial.println(currentTouchValue);

  server.send(200, "text/plain", String(currentTouchValue));
}

// 可选：返回 JSON 格式数据
void handleTouchJson() {
  currentTouchValue = touchRead(TOUCH_PIN);

  String json = "{";
  json += "\"touchValue\":";
  json += currentTouchValue;
  json += "}";

  server.send(200, "application/json", json);
}

// 404
void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found");
}

// ================= setup / loop =================

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println();
  Serial.println("ESP32 Touch Web Dashboard Started");
  Serial.print("AP SSID: ");
  Serial.println(ap_ssid);
  Serial.print("AP Password: ");
  Serial.println(ap_pass);
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/touch", handleTouchValue);
  server.on("/touchJson", handleTouchJson);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}