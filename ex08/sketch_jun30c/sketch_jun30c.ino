// ex08：物联网安防报警器模拟实验
// 参考基础：基础IO + 触摸引脚 + Web服务器
// 功能：网页布防/撤防；布防状态下触摸引脚触发报警；报警锁定直到网页撤防

#include <WiFi.h>
#include <WebServer.h>

// ================= WiFi AP 配置 =================
// ESP32 会创建自己的热点
const char* ap_ssid = "ESP32-ALARM";
const char* ap_pass = "12345678";   // 至少8位

WebServer server(80);

// ================= 引脚配置 =================
// ESP32 T0 触摸通道对应 GPIO4
#define TOUCH_PIN 4

// ESP32 DevKit 常见板载 LED 是 GPIO2
#define LED_PIN 2

// 触摸阈值：触摸时 touchRead() 的值会变小
// 需要根据串口监视器观察调整
int threshold = 500;

// ================= 系统状态变量 =================
// 是否布防
bool isArmed = false;

// 是否已经触发报警
bool isAlarmTriggered = false;

// ================= LED 闪烁控制 =================
bool ledState = false;
unsigned long lastBlinkTime = 0;

// 报警闪烁间隔，数值越小闪得越快
const unsigned long alarmBlinkInterval = 80;

// ================= 生成网页 =================
String makePage() {
  String armText = isArmed ? "已布防" : "未布防";
  String alarmText = isAlarmTriggered ? "报警中" : "正常";
  String statusClass = isAlarmTriggered ? "alarm" : (isArmed ? "armed" : "safe");

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 安防报警器</title>
  <style>
    body {
      margin: 0;
      padding: 0;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Arial, sans-serif;
      background: linear-gradient(135deg, #eef2ff, #f8fafc);
      color: #111827;
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
    }

    .card {
      width: 88%;
      max-width: 430px;
      background: #ffffff;
      border-radius: 24px;
      padding: 28px;
      box-shadow: 0 18px 45px rgba(0, 0, 0, 0.12);
      text-align: center;
    }

    h1 {
      margin-top: 0;
      font-size: 26px;
      color: #1f2937;
    }

    .subtitle {
      color: #6b7280;
      font-size: 15px;
      margin-bottom: 24px;
      line-height: 1.6;
    }

    .status {
      border-radius: 18px;
      padding: 18px;
      margin-bottom: 24px;
      font-size: 18px;
      font-weight: 700;
    }

    .safe {
      background: #ecfdf5;
      color: #047857;
    }

    .armed {
      background: #eff6ff;
      color: #1d4ed8;
    }

    .alarm {
      background: #fef2f2;
      color: #dc2626;
      animation: blink 0.5s infinite;
    }

    @keyframes blink {
      0% { opacity: 1; }
      50% { opacity: 0.45; }
      100% { opacity: 1; }
    }

    .info {
      font-size: 15px;
      color: #374151;
      line-height: 1.8;
      margin-bottom: 24px;
    }

    .buttons {
      display: flex;
      gap: 12px;
    }

    button {
      flex: 1;
      border: none;
      border-radius: 16px;
      padding: 14px 0;
      font-size: 16px;
      font-weight: 600;
      cursor: pointer;
    }

    .arm-btn {
      background: #2563eb;
      color: white;
    }

    .disarm-btn {
      background: #ef4444;
      color: white;
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
    <h1>ESP32 安防报警器</h1>
    <div class="subtitle">网页端控制布防 / 撤防，触摸引脚模拟入侵检测</div>

    <div class="status STATUS_CLASS">
      系统状态：ARM_TEXT<br>
      报警状态：ALARM_TEXT
    </div>

    <div class="info">
      未布防：触摸无反应<br>
      已布防：触摸后报警锁定<br>
      撤防后：LED熄灭，状态重置
    </div>

    <div class="buttons">
      <button class="arm-btn" onclick="location.href='/arm'">布防 Arm</button>
      <button class="disarm-btn" onclick="location.href='/disarm'">撤防 Disarm</button>
    </div>

    <div class="tip">
      手机或电脑连接 ESP32-ALARM 热点后，访问 192.168.4.1
    </div>
  </div>
</body>
</html>
)rawliteral";

  html.replace("STATUS_CLASS", statusClass);
  html.replace("ARM_TEXT", armText);
  html.replace("ALARM_TEXT", alarmText);

  return html;
}

// ================= Web 路由处理 =================
void handleRoot() {
  server.send(200, "text/html", makePage());
}

void handleArm() {
  isArmed = true;
  isAlarmTriggered = false;
  ledState = false;
  digitalWrite(LED_PIN, LOW);

  Serial.println("System Armed");

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDisarm() {
  isArmed = false;
  isAlarmTriggered = false;
  ledState = false;
  digitalWrite(LED_PIN, LOW);

  Serial.println("System Disarmed, Alarm Reset");

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleStatus() {
  String json = "{";
  json += "\"armed\":";
  json += isArmed ? "true" : "false";
  json += ",";
  json += "\"alarm\":";
  json += isAlarmTriggered ? "true" : "false";
  json += "}";

  server.send(200, "application/json", json);
}

void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found");
}

// ================= 触摸检测 =================
void checkTouchSensor() {
  int touchValue = touchRead(TOUCH_PIN);
  bool isTouched = touchValue < threshold;

  // 只有布防状态下，触摸才会触发报警
  if (isArmed && isTouched) {
    if (!isAlarmTriggered) {
      isAlarmTriggered = true;
      Serial.println("Alarm Triggered!");
    }
  }

  // 串口输出，方便观察触摸值和状态
  Serial.print("Touch Value: ");
  Serial.print(touchValue);
  Serial.print(" | Armed: ");
  Serial.print(isArmed);
  Serial.print(" | Alarm: ");
  Serial.println(isAlarmTriggered);

  delay(50);
}

// ================= 报警LED闪烁 =================
void updateAlarmLed() {
  // 报警状态：LED 高频闪烁
  if (isAlarmTriggered) {
    unsigned long currentTime = millis();

    if (currentTime - lastBlinkTime >= alarmBlinkInterval) {
      lastBlinkTime = currentTime;

      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
  } 
  // 非报警状态：LED熄灭
  else {
    ledState = false;
    digitalWrite(LED_PIN, LOW);
  }
}

// ================= setup / loop =================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println();
  Serial.println("ESP32 IoT Security Alarm Started");
  Serial.print("AP SSID: ");
  Serial.println(ap_ssid);
  Serial.print("AP Password: ");
  Serial.println(ap_pass);
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/status", handleStatus);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  checkTouchSensor();

  updateAlarmLed();
}