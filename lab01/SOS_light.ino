// 定义 LED 引脚，ESP32 通常板载 LED 连接在 GPIO 2
const int ledPin = 2;

void setup() {
  // 初始化串口通信，设置波特率为 115200
  Serial.begin(115200);

  // 将 LED 引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
}

void shortBlink() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void longBlink() {
  digitalWrite(ledPin, HIGH);
  delay(600);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void loop() {
  Serial.println("SOS Start");

  // S：短闪 3 次
  Serial.println("S");
  for (int i = 0; i < 3; i++) {
    shortBlink();
  }

  delay(500); // 字母间隔

  // O：长闪 3 次
  Serial.println("O");
  for (int i = 0; i < 3; i++) {
    longBlink();
  }

  delay(500); // 字母间隔

  // S：短闪 3 次
  Serial.println("S");
  for (int i = 0; i < 3; i++) {
    shortBlink();
  }

  Serial.println("SOS End");

  delay(2000); // 单词间隔
}