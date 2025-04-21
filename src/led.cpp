/*
 * MIT License
 *
 * Copyright (c) 2024 Kouhei Ito
 * Copyright (c) 2024 M5Stack
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "led.hpp"
#include "sensor.hpp"
#include "rc.hpp"
#include "flight_control.hpp"

CRGB Led_color       = CRGB::Black;
uint32_t Led_color2      = 255;
CRGB Led_color3      = CRGB::Black;
uint16_t LedBlinkCounter = 0;
// CRGB led_esp[1];
CRGB led_onboard[2];
CRGB last_led_state[3];

void led_init(void) {
    FastLED.addLeds<WS2812, PIN_LED_ONBORD, GRB>(led_onboard, 2);
    // FastLED.addLeds<WS2812, PIN_LED_ESP, GRB>(led_esp, 1);
    FastLED.setBrightness(15);
}

bool leds_changed(void) {
    // return memcmp(led_onboard, &last_led_state[0], sizeof(led_onboard)) != 0 ||
    //        memcmp(led_esp, &last_led_state[2], sizeof(led_esp)) != 0;
    return memcmp(led_onboard, &last_led_state[0], sizeof(led_onboard)) != 0;
}

void led_show(uint8_t temp_brightness) {
    if (leds_changed()) {
        uint8_t original = FastLED.getBrightness();
        FastLED.setBrightness(temp_brightness);
        FastLED.show();
        FastLED.setBrightness(original);
        memcpy(&last_led_state[0], led_onboard, sizeof(led_onboard));
        // memcpy(&last_led_state[2], led_esp, sizeof(led_esp));
    }
}

void led_drive(void) {
    if (Mode == AVERAGE_MODE) {
        onboard_led1(COLOR_PURPLE, 1);
        onboard_led2(COLOR_PURPLE, 1);
    } else if (Mode == AUTO_LANDING_MODE) {
        onboard_led1(COLOR_GREEN, 1);
        onboard_led2(COLOR_GREEN, 1);
    } else if (Mode == FLIGHT_MODE) {
        if (Control_mode == ANGLECONTROL) {
            if (Flip_flag == 0)
                Led_color = COLOR_YELLOW;  // 稳定模式/手动飞行时为黄色
            else
                Led_color = COLOR_FLIP;  // 翻滚时为橙色
        } else{
            Led_color = COLOR_ACRO;  // 自由模式（Acro）使用粉紫色
        }
        if (Throttle_control_mode == 1) Led_color = 0xc71585;  // 高度控制初始化阶段为紫红色
        if (Alt_flag >= 1) Led_color = 0x331155;               // 高度控制模式中为暗紫色
        if (Rc_err_flag == 1) Led_color = 0xff0000;            // 遥控丢失为红色

        if (Under_voltage_flag < UNDER_VOLTAGE_COUNT) {
            onboard_led1(Led_color, 1);
            onboard_led2(Led_color, 1);
        } else {
            onboard_led1(COLOR_POWEROFF, 1);
            onboard_led2(Led_color, 1);
        }
    } else if (Mode == PARKING_MODE) {
        if (Under_voltage_flag < UNDER_VOLTAGE_COUNT) {
            // 彩灯滚动动画
            if (LedBlinkCounter == 0) {  //<10
                if (Led_color2 & 0x800000)
                    Led_color2 = (Led_color2 << 1) | 1;
                else
                    Led_color2 = Led_color2 << 1;
                onboard_led1(Led_color2, 1);
                onboard_led2(Led_color2, 1);
                LedBlinkCounter++;
            }
            LedBlinkCounter++;
            if (LedBlinkCounter > 20) LedBlinkCounter = 0;
        } else {
            // 水蓝色闪烁效果
            if (LedBlinkCounter < 10) {
                onboard_led1(COLOR_POWEROFF, 1);
                onboard_led2(COLOR_POWEROFF, 1);
            } else if (LedBlinkCounter < 200) {
                onboard_led1(COLOR_POWEROFF, 0);
                onboard_led2(COLOR_POWEROFF, 0);
            } else{
                LedBlinkCounter = 0;
            }
            LedBlinkCounter++;
        }
    }
    // LED show
    led_show(32);
}

void onboard_led1(CRGB p, uint8_t state) {
    if (state == 1) {
        led_onboard[0] = p;
    } else {
        led_onboard[0] = 0;
    }
    return;
}

void onboard_led2(CRGB p, uint8_t state) {
    if (state == 1) {
        led_onboard[1] = p;
    } else {
        led_onboard[1] = 0;
    }
    return;
}

void esp_led(CRGB p, uint8_t state) {
    // if (state == 1)
    //     led_esp[0] = p;
    // else
    //     led_esp[0] = 0;
    // return;

    // xiao_plus上为单色灯，仅使用亮灭，保留接口
    pinMode(PIN_LED_ESP, OUTPUT);  // 确保设置为输出模式
    if (state == 1)
        digitalWrite(PIN_LED_ESP, HIGH);  // 点亮板载灯
    else
        digitalWrite(PIN_LED_ESP, LOW);   // 熄灭板载灯
}
