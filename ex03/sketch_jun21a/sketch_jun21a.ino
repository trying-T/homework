// ESP32 DevKit 板载 LED 通常是 GPIO2
#define LED_PIN 2

// 时间参数，单位：毫秒
const unsigned long DOT_TIME = 200;      // 短闪
const unsigned long DASH_TIME = 600;     // 长闪
const unsigned long SYMBOL_GAP = 200;    // 同一字母内闪烁间隔
const unsigned long LETTER_GAP = 600;    // 字母之间间隔
const unsigned long SOS_GAP = 2000;      // 一轮 SOS 结束后的长停顿

// 用数组描述整个 SOS 闪烁序列
// true  表示 LED 亮
// false 表示 LED 灭
bool ledPattern[] = {
  // S: · · ·
  true,  false,
  true,  false,
  true,  false,

  // O: — — —
  true,  false,
  true,  false,
  true,  false,

  // S: · · ·
  true,  false,
  true,  false,
  true,  false
};

// 每个状态持续的时间
unsigned long durationPattern[] = {
  // S: 三次短闪
  DOT_TIME, SYMBOL_GAP,
  DOT_TIME, SYMBOL_GAP,
  DOT_TIME, LETTER_GAP,

  // O: 三次长闪
  DASH_TIME, SYMBOL_GAP,
  DASH_TIME, SYMBOL_GAP,
  DASH_TIME, LETTER_GAP,

  // S: 三次短闪
  DOT_TIME, SYMBOL_GAP,
  DOT_TIME, SYMBOL_GAP,
  DOT_TIME, SOS_GAP
};

// 当前执行到序列中的第几个状态
int patternIndex = 0;

// 记录上一次状态切换的时间
unsigned long previousMillis = 0;

// 自动计算序列长度
const int patternLength = sizeof(ledPattern) / sizeof(ledPattern[0]);

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // 初始状态
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  // 判断当前状态持续时间是否已经结束
  if (currentMillis - previousMillis >= durationPattern[patternIndex]) {
    previousMillis = currentMillis;

    // 切换到下一个状态
    patternIndex++;

    // 如果一轮 SOS 播放完毕，就回到开头
    if (patternIndex >= patternLength) {
      patternIndex = 0;
    }

    // 根据当前状态控制 LED
    digitalWrite(LED_PIN, ledPattern[patternIndex]);
  }
}
