#pragma once

#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "config.h"

bool setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // 1600x1200
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Blad inicjalizacji kamery: 0x%x\n", err);
    return false;
  }
  
  sensor_t * s = esp_camera_sensor_get();
  s->set_vflip(s, 0); // Zmień na 1 jeśli obraz jest do góry nogami
  s->set_hmirror(s, 0);
  return true;
}

bool setupSD() {
  // Tryb 1-bit, zwalnia piny 12 i 13
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("Blad montowania karty SD!");
    return false;
  }
  Serial.println("Karta SD podlaczona poprawnie.");
  return true;
}

String savePhotoToSD(camera_fb_t * fb, String timestamp) {
  if (!fb) return "";
  
  String path = "/foto_" + timestamp + ".jpg";
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Nie udalo sie otworzyc pliku do zapisu na SD");
    return "";
  }
  file.write(fb->buf, fb->len);
  file.close();
  Serial.println("Zdjecie zapisane na SD: " + path);
  return path;
}
