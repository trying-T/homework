// ESP32 DevKit 板载 LED 通常是 GPIO2
#define LED_PIN 2

// 记录上一次 LED 状态改变的时间
unsigned long previousMillis = 0;

// 1Hz 闪烁：亮500ms，灭500ms
const unsigned long interval = 500;

// 记录 LED 当前状态
bool ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // 获取当前系统运行时间，单位是毫秒
  unsigned long currentMillis = millis();

  // 判断是否已经过了 500ms
  if (currentMillis - previousMillis >= interval) {
    // 更新上一次变化时间
    previousMillis = currentMillis;

    // 翻转 LED 状态
    ledState = !ledState;

    // 输出到 LED
    digitalWrite(LED_PIN, ledState);
  }
}