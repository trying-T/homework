// ex06：警车双闪灯效（双通道PWM）
// 功能：两个LED反相渐变，一个逐渐变亮时，另一个逐渐变暗

#include <Arduino.h>

// 两个 LED 引脚
#define LED_A_PIN 18
#define LED_B_PIN 19

// PWM 通道
#define PWM_CHANNEL_A 0
#define PWM_CHANNEL_B 1

// PWM 参数
#define PWM_FREQ 5000        // PWM频率 5kHz
#define PWM_RESOLUTION 8     // 8位分辨率，占空比范围 0~255

// 渐变速度，数值越小变化越快
int fadeDelay = 2;

// 兼容 ESP32 Arduino Core 2.x 和 3.x 的 PWM 写入函数
void writePwm(int channel, int duty) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWriteChannel(channel, duty);
#else
  ledcWrite(channel, duty);
#endif
}

void setup() {
  Serial.begin(115200);

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  // ESP32 Arduino Core 3.x 写法
  ledcAttachChannel(LED_A_PIN, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL_A);
  ledcAttachChannel(LED_B_PIN, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL_B);
#else
  // ESP32 Arduino Core 2.x 写法
  ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(LED_A_PIN, PWM_CHANNEL_A);
  ledcAttachPin(LED_B_PIN, PWM_CHANNEL_B);
#endif

  Serial.println("ex06 Dual PWM Police Light Start");
}

void loop() {
  // A 从暗到亮，B 从亮到暗
  for (int duty = 0; duty <= 255; duty++) {
    writePwm(PWM_CHANNEL_A, duty);
    writePwm(PWM_CHANNEL_B, 255 - duty);
    delay(fadeDelay);
  }

  // A 从亮到暗，B 从暗到亮
  for (int duty = 255; duty >= 0; duty--) {
    writePwm(PWM_CHANNEL_A, duty);
    writePwm(PWM_CHANNEL_B, 255 - duty);
    delay(fadeDelay);
  }
}