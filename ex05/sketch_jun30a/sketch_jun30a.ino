// ex05：多档位触摸调速呼吸灯
// 关联基础：实验3 PWM呼吸灯 + 实验4 触摸引脚
// 功能：LED持续呼吸，每触摸一次，呼吸速度在 1 -> 2 -> 3 -> 1 之间循环切换

// ESP32 T0 触摸通道对应 GPIO4
#define TOUCH_PIN 4

// ESP32 DevKit 常见板载 LED 是 GPIO2
#define LED_PIN 2

// 触摸阈值：触摸时 touchRead() 的值会变小
// 需要根据串口监视器观察调整
int threshold = 400;

// 当前速度档位：1慢速，2中速，3快速
int speedLevel = 1;

// ---------- 触摸防抖相关变量 ----------
bool stableTouchState = false;      // 稳定后的触摸状态
bool lastRawTouchState = false;     // 上一次原始触摸状态
unsigned long lastDebounceTime = 0; // 上一次状态变化时间
const unsigned long debounceDelay = 80; // 防抖时间，单位ms

// ---------- 呼吸灯相关变量 ----------
int brightness = 0;        // 当前亮度，范围 0~255
int fadeDirection = 1;     // 亮度变化方向：1表示变亮，-1表示变暗

unsigned long lastBreathTime = 0;   // 上一次更新亮度的时间

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  // 初始 LED 熄灭
  analogWrite(LED_PIN, 0);

  delay(1000);
  Serial.println("ex05 Touch Speed-Controlled Breathing LED Start");
}

void loop() {
  handleTouch();   // 检测触摸，切换速度档位
  updateBreath();  // 根据当前档位更新呼吸灯
}

// 处理触摸输入：带防抖 + 边缘检测
void handleTouch() {
  int touchValue = touchRead(TOUCH_PIN);

  // ESP32 触摸时数值会下降
  bool rawTouchState = touchValue < threshold;

  // 如果原始状态发生变化，重置防抖计时
  if (rawTouchState != lastRawTouchState) {
    lastDebounceTime = millis();
    lastRawTouchState = rawTouchState;
  }

  // 超过防抖时间后，认为状态稳定
  if (millis() - lastDebounceTime > debounceDelay) {

    // 如果稳定状态发生变化
    if (rawTouchState != stableTouchState) {

      bool previousStableState = stableTouchState;
      stableTouchState = rawTouchState;

      // 只在“未触摸 -> 被触摸”的瞬间切换档位
      if (previousStableState == false && stableTouchState == true) {

        speedLevel++;

        if (speedLevel > 3) {
          speedLevel = 1;
        }

        Serial.print("Touch triggered. Current speed level: ");
        Serial.println(speedLevel);
      }
    }
  }

  // 串口输出触摸值，方便调 threshold
  Serial.print("Touch Value: ");
  Serial.print(touchValue);
  Serial.print(" | Speed Level: ");
  Serial.println(speedLevel);
}

// 根据当前速度档位更新呼吸灯
void updateBreath() {
  unsigned long currentTime = millis();

  // 不同档位对应不同的更新时间间隔
  // 数值越小，呼吸越快
  int breathInterval;

  if (speedLevel == 1) {
    breathInterval = 25;   // 慢速
  } else if (speedLevel == 2) {
    breathInterval = 8;    // 中速
  } else {
    breathInterval = 1;    // 快速
  }

  // 到达更新时间后，改变一次亮度
  if (currentTime - lastBreathTime >= breathInterval) {
    lastBreathTime = currentTime;

    brightness += fadeDirection;

    // 到达最亮或最暗时，反向变化
    if (brightness >= 255) {
      brightness = 255;
      fadeDirection = -1;
    }

    if (brightness <= 0) {
      brightness = 0;
      fadeDirection = 1;
    }

    analogWrite(LED_PIN, brightness);
  }
}