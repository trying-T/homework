// ESP32 触摸自锁开关实验
// 功能：摸一下触摸引脚，LED亮并保持；再摸一下，LED灭并保持

// ESP32 T0 对应 GPIO4
#define TOUCH_PIN 4

// ESP32 DevKit 常见板载 LED 是 GPIO2
#define LED_PIN 2

// 触摸阈值：touchRead() 的值在触摸时会变小
// 这个值需要根据串口监视器观察调整
int threshold = 400;

// LED 当前状态
bool ledState = false;

// 稳定后的触摸状态
bool stableTouchState = false;

// 上一次读取到的原始触摸状态
bool lastRawTouchState = false;

// 防抖计时
unsigned long lastDebounceTime = 0;

// 防抖时间，单位 ms
const unsigned long debounceDelay = 80;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(1000);
  Serial.println("ESP32 Touch Lock Switch Start");
}

void loop() {
  // 读取触摸值
  int touchValue = touchRead(TOUCH_PIN);

  // 判断当前是否被触摸
  // ESP32 触摸时 touchValue 会下降
  bool rawTouchState = touchValue < threshold;

  // 如果原始状态发生变化，说明可能是按下或松开，也可能是抖动
  if (rawTouchState != lastRawTouchState) {
    lastDebounceTime = millis();
    lastRawTouchState = rawTouchState;
  }

  // 超过防抖时间后，才认为状态稳定
  if (millis() - lastDebounceTime > debounceDelay) {

    // 如果稳定状态真的发生变化
    if (rawTouchState != stableTouchState) {

      // 记录旧状态
      bool previousStableState = stableTouchState;

      // 更新稳定状态
      stableTouchState = rawTouchState;

      // 只在“未触摸 -> 被触摸”的瞬间翻转 LED
      if (previousStableState == false && stableTouchState == true) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);

        Serial.print("Touch triggered, LED = ");
        Serial.println(ledState ? "ON" : "OFF");
      }
    }
  }

  // 串口观察触摸值，方便调整 threshold
  Serial.print("Touch Value: ");
  Serial.print(touchValue);
  Serial.print(" | Raw Touch: ");
  Serial.print(rawTouchState);
  Serial.print(" | LED State: ");
  Serial.println(ledState);

  delay(50);
}