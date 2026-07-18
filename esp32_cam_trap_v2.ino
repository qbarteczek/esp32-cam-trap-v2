#include <Arduino.h>

bool isWebServerMode = false;
WiFiClientSecure secured_client;
UniversalTelegramBot bot("", secured_client); // Bedzie nadpisany z preferences
WebServer server(80);

#include "config.h"
#include "network_ota.h"
#include "camera_sd.h"
#include "telegram_logic.h"
#include "web_server.h"

void goToDeepSleep() {
  Serial.println("Kamera idzie spac. Wybudzenie ze styków PIR (GPIO13).");
  esp_sleep_enable_ext0_wakeup(PIR_PIN, 1);
  delay(100);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start V2...");

  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);
  delay(100);

  loadPreferences();
  bot.updateToken(botToken);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  if (!setupCamera()) {
    Serial.println("Kamera zablokowana, usypiam.");
    goToDeepSleep();
  }

  if (!setupSD()) {
    Serial.println("Brak karty SD, usypiam.");
    goToDeepSleep();
  }

  if (digitalRead(MODE_SWITCH_PIN) == LOW) {
    isWebServerMode = true;
    Serial.println("TRYB: Web Server (Serwisowy/OTA)");
    if (!connectWiFi()) {
      startAPMode();
    } else {
      setupNTP();
      setupOTA();
      setupWebServer();
    }
  } else {
    isWebServerMode = false;
    Serial.println("TRYB: Fotopułapka");
    
    if (connectWiFi()) {
      setupNTP();
    }

    if (USE_FLASH) {
      pinMode(FLASH_LED_PIN, OUTPUT);
      digitalWrite(FLASH_LED_PIN, HIGH);
      delay(500);
    }
    
    camera_fb_t * fb = esp_camera_fb_get();
    
    if (USE_FLASH) {
      digitalWrite(FLASH_LED_PIN, LOW);
    }

    if (fb) {
      String timestamp = getFormattedTime();
      savePhotoToSD(fb, timestamp);
      sendPhotoToTelegram(fb);
      esp_camera_fb_return(fb);

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Czekam 10s na ewentualne komendy Telegram (np. /live)...");
        long start = millis();
        while(millis() - start < 10000) {
          checkTelegramMessages();
          delay(1000);
          if (isWebServerMode) {
             setupOTA();
             setupWebServer();
             break; // Wychodzimy, bo z komendy uruchomilismy Web Server
          }
        }
      }
    }
    
    if (!isWebServerMode) {
      goToDeepSleep();
    }
  }
}

void loop() {
  if (isWebServerMode) {
    server.handleClient();
    handleOTA();
    checkTelegramMessages(); // Bot moze odpowiadac tez w trybie serwera
    delay(10);
  }
}
