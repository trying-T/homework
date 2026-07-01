// ex07：Web网页端无极调光器
// 功能：手机/电脑访问 ESP32 网页，通过滑动条实时控制 LED 亮度
// 关联基础：PWM呼吸灯 + Web服务器

#include <WiFi.h>
#include <WebServer.h>

// ================= WiFi AP 配置 =================
// ESP32 会创建一个热点，手机/电脑连接这个热点
const char* ap_ssid = "ESP32-DIMMER";
const char* ap_pass = "12345678";   // 至少8位

WebServer server(80);

// ================= LED / PWM 配置 =================
// ESP32 DevKit 常见板载LED为 GPIO2
// 如果你使用外接LED，可以改成 18、19、21 等GPIO
#define LED_PIN 2

#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8   // 8位分辨率，占空比范围 0~255

int currentBrightness = 0;

// ================= PWM 兼容函数 =================
// 兼容 ESP32 Arduino Core 2.x 和 3.x
void pwmInit() {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttachChannel(LED_PIN, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL);
#else
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, PWM_CHANNEL);
#endif
}

void pwmWriteValue(int value) {
  value = constrain(value, 0, 255);

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWriteChannel(PWM_CHANNEL, value);
#else
  ledcWrite(PWM_CHANNEL, value);
#endif

  currentBrightness = value;
}

// ================= 网页HTML =================
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 无极调光器</title>
  <style>
    body {
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Arial, sans-serif;
      background: linear-gradient(135deg, #e8f0ff, #f7f9fc);
      color: #1f2937;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
    }

    .card {
      width: 88%;
      max-width: 420px;
      background: white;
      border-radius: 24px;
      padding: 28px;
      box-shadow: 0 18px 45px rgba(0, 0, 0, 0.12);
      text-align: center;
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
    }

    .value-box {
      font-size: 48px;
      font-weight: 700;
      color: #2563eb;
      margin: 18px 0 8px;
    }

    .label {
      color: #6b7280;
      font-size: 14px;
      margin-bottom: 28px;
    }

    input[type="range"] {
      width: 100%;
      height: 8px;
      border-radius: 999px;
      background: #dbeafe;
      outline: none;
      -webkit-appearance: none;
    }

    input[type="range"]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 28px;
      height: 28px;
      border-radius: 50%;
      background: #2563eb;
      cursor: pointer;
      box-shadow: 0 4px 12px rgba(37, 99, 235, 0.45);
    }

    input[type="range"]::-moz-range-thumb {
      width: 28px;
      height: 28px;
      border-radius: 50%;
      background: #2563eb;
      cursor: pointer;
      border: none;
    }

    .buttons {
      display: flex;
      gap: 10px;
      margin-top: 24px;
    }

    button {
      flex: 1;
      border: none;
      border-radius: 14px;
      padding: 12px 0;
      font-size: 15px;
      cursor: pointer;
      background: #eff6ff;
      color: #1d4ed8;
    }

    button:active {
      transform: scale(0.97);
    }

    .tip {
      margin-top: 24px;
      color: #9ca3af;
      font-size: 13px;
      line-height: 1.6;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>ESP32 无极调光器</h1>
    <div class="subtitle">拖动滑动条实时调节 LED 亮度</div>

    <div class="value-box" id="valueText">0</div>
    <div class="label">当前PWM占空比：0 ~ 255</div>

    <input 
      type="range" 
      id="brightnessSlider" 
      min="0" 
      max="255" 
      value="0"
    >

    <div class="buttons">
      <button onclick="setBrightness(0)">关闭</button>
      <button onclick="setBrightness(128)">半亮</button>
      <button onclick="setBrightness(255)">最亮</button>
    </div>

    <div class="tip">
      手机或电脑连接 ESP32-DIMMER 热点后，访问 192.168.4.1
    </div>
  </div>

  <script>
    const slider = document.getElementById("brightnessSlider");
    const valueText = document.getElementById("valueText");

    function sendBrightness(value) {
      valueText.innerText = value;

      fetch("/set?value=" + value)
        .then(response => response.text())
        .then(data => {
          console.log("ESP32 response:", data);
        })
        .catch(error => {
          console.log("Error:", error);
        });
    }

    slider.addEventListener("input", function() {
      sendBrightness(this.value);
    });

    function setBrightness(value) {
      slider.value = value;
      sendBrightness(value);
    }
  </script>
</body>
</html>
)rawliteral";

  return html;
}

// ================= HTTP 路由处理 =================
void handleRoot() {
  server.send(200, "text/html", makePage());
}

void handleSetBrightness() {
  if (server.hasArg("value")) {
    int value = server.arg("value").toInt();
    value = constrain(value, 0, 255);

    pwmWriteValue(value);

    Serial.print("Brightness set to: ");
    Serial.println(value);

    server.send(200, "text/plain", "OK: " + String(value));
  } else {
    server.send(400, "text/plain", "Missing value");
  }
}

void handleState() {
  server.send(200, "text/plain", String(currentBrightness));
}

void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found");
}

// ================= setup / loop =================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pwmInit();
  pwmWriteValue(0);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println();
  Serial.println("ESP32 Web PWM Dimmer Started");
  Serial.print("AP SSID: ");
  Serial.println(ap_ssid);
  Serial.print("AP Password: ");
  Serial.println(ap_pass);
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetBrightness);
  server.on("/state", handleState);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}