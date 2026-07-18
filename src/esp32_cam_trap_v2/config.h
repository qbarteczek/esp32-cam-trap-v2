#pragma once

#include <Arduino.h>

// ==========================================
// KONFIGURACJA PINÓW
// ==========================================
const int MODE_SWITCH_PIN = 12; // Pin decydujący o trybie (GND = WebServer, NC = Fotopułapka)
const gpio_num_t PIR_PIN = GPIO_NUM_13; // Sygnał z czujnika PIR
const int FLASH_LED_PIN = 4; // Wbudowana dioda (flash)
const bool USE_FLASH = false; // Zmień na true, aby doświetlać nocne zdjęcia

// Piny kamery AI-Thinker ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ==========================================
// STATUS SYSTEMU
// ==========================================
extern bool isWebServerMode;
