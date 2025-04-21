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

#ifndef LED_HPP
#define LED_HPP

#include <FastLED.h>
#include <stdint.h>

// #define WHITE         0xffffff
// #define BLUE          0x0000ff
// #define RED           0xff0000
// #define YELLOW        0xffff00
// #define GREEN         0x00ff00
// #define PERPLE        0xff00ff
// #define POWEROFFCOLOR 0x18EBF9
// #define FLIPCOLOR     0xFF9933

constexpr CRGB COLOR_BLUE       = CRGB(0, 0, 255);
constexpr CRGB COLOR_RED        = CRGB(255, 0, 0);
constexpr CRGB COLOR_GREEN      = CRGB(0, 255, 0);
constexpr CRGB COLOR_WHITE      = CRGB(255, 255, 255);
constexpr CRGB COLOR_PURPLE     = CRGB(255, 0, 255);
constexpr CRGB COLOR_YELLOW     = CRGB(255, 255, 0);
constexpr CRGB COLOR_FLIP       = CRGB(255, 153, 51);   // 0xFF9933
constexpr CRGB COLOR_ACRO       = CRGB(220, 102, 155);  // 0xDC669B
constexpr CRGB COLOR_ALT_INIT   = CRGB(199, 21, 133);   // 0xc71585
constexpr CRGB COLOR_ALT_HOLD   = CRGB(51, 17, 85);     // 0x331155
constexpr CRGB COLOR_RC_LOST    = CRGB(255, 0, 0);      // 0xff0000
constexpr CRGB COLOR_POWEROFF   = CRGB(24, 235, 249);   // 0x18EBF9

#define PIN_LED_ONBORD 9
#define PIN_LED_ESP    21
#define NUM_LEDS       3

extern CRGB Led_color;

void led_init(void);
bool leds_changed(void);
void led_show(uint8_t temp_brightness = 32);
void led_drive(void);
void onboard_led1(CRGB p, uint8_t state);
void onboard_led2(CRGB p, uint8_t state);
void esp_led(CRGB p, uint8_t state);

#endif