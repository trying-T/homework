#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";
const int LED_PIN = 2; // D2 常见为 GPIO2

WebServer server(80);

String makePage() {
  String state = digitalRead(LED_PIN) ? "ON" : "OFF";
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实验2</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>第二部分：按钮控制 LED</h1>
  <p>当前状态：<b>)rawliteral" + state + R"rawliteral(</b></p>
  <a href="/on"><button style="padding:10px 20px;">点亮 LED</button></a>
  <a href="/off"><button style="padding:10px 20px;">熄灭 LED</button></a>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleOn() {
  digitalWrite(LED_PIN, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
