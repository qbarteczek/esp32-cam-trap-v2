#pragma once

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include "network_ota.h"

extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;
extern bool isWebServerMode;

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != chatId) {
      bot.sendMessage(chat_id, "Brak autoryzacji.", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println("Otrzymano komende: " + text);

    if (text == "/status") {
      String welcome = "Witaj!\n";
      welcome += "Fotopulapka jest online.\n";
      welcome += "IP: " + WiFi.localIP().toString() + "\n";
      bot.sendMessage(chat_id, welcome, "");
    }
    else if (text == "/live") {
      bot.sendMessage(chat_id, "Przepinam w tryb Web Servera. Otworz podany adres IP w przegladarce.", "");
      isWebServerMode = true; // Zostanie przetworzone w glównej petli
    }
    else {
      bot.sendMessage(chat_id, "Nieznana komenda. Dostepne: /status, /live", "");
    }
  }
}

void checkTelegramMessages() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}

void sendPhotoToTelegram(camera_fb_t * fb) {
  if (WiFi.status() != WL_CONNECTED || botToken == "" || chatId == "") {
    Serial.println("Telegram: Brak WiFi lub konfiguracji.");
    return;
  }
  
  Serial.println("Wysylanie zdjecia na Telegram...");
  
  String getAll;
  String getBody;
  
  secured_client.println("POST /bot" + botToken + "/sendPhoto HTTP/1.1");
  secured_client.println("Host: api.telegram.org");
  secured_client.println("Connection: close");
  secured_client.println("Content-Type: multipart/form-data; boundary=Esp32CamBoundary");
  
  String head = "--Esp32CamBoundary\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + chatId + "\r\n";
  head += "--Esp32CamBoundary\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--Esp32CamBoundary--\r\n";
  
  uint32_t contentLength = head.length() + fb->len + tail.length();
  secured_client.println("Content-Length: " + String(contentLength));
  secured_client.println();
  secured_client.print(head);
  
  uint8_t *fbBuf = fb->buf;
  size_t fbLen = fb->len;
  for (size_t n=0; n<fbLen; n=n+1024) {
    if (n+1024 < fbLen) {
      secured_client.write(fbBuf, 1024);
      fbBuf += 1024;
    } else if (fbLen%1024>0) {
      size_t remainder = fbLen%1024;
      secured_client.write(fbBuf, remainder);
    }
  }   
  secured_client.print(tail);
  
  int waitTime = 10000;
  long startTimer = millis();
  boolean state = false;
  
  while ((startTimer + waitTime) > millis()){
    delay(100);      
    while (secured_client.available()) {
      char c = secured_client.read();
      if (state==true) getBody += String(c);        
      if (c == '\n') {
        if (getAll.length()==0) state=true; 
        getAll = "";
      } else if (c != '\r') {
        getAll += String(c);
      }
      startTimer = millis();
    }
    if (getBody.length()>0) break;
  }
  Serial.println("Zdjecie wyslane pomyślnie.");
}
