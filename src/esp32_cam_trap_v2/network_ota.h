#pragma once

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>
#include <Preferences.h>
#include "time.h"

Preferences preferences;
String ssid = "";
String password = "";
String botToken = "";
String chatId = "";

void loadPreferences() {
  preferences.begin("cam-config", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");
  botToken = preferences.getString("botToken", "");
  chatId = preferences.getString("chatId", "");
  preferences.end();
}

void savePreferences(String newSsid, String newPass, String newToken, String newChatId) {
  preferences.begin("cam-config", false);
  preferences.putString("ssid", newSsid);
  preferences.putString("password", newPass);
  preferences.putString("botToken", newToken);
  preferences.putString("chatId", newChatId);
  preferences.end();
  
  // Update running variables
  ssid = newSsid;
  password = newPass;
  botToken = newToken;
  chatId = newChatId;
}

bool connectWiFi() {
  if (ssid == "") return false;
  
  Serial.print("Laczenie z Wi-Fi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi podlaczone! IP: " + WiFi.localIP().toString());
    return true;
  }
  Serial.println("\nNie udalo sie polaczyc z Wi-Fi.");
  return false;
}

void setupNTP() {
  // Inicjalizacja czasu (NTP)
  configTime(3600, 3600, "pool.ntp.org", "time.nist.gov"); // Strefa czasowa Polska (GMT+1, letni GMT+2)
  Serial.println("Czas NTP skonfigurowany.");
}

String getFormattedTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return String(millis()); // Zwróć millis jeśli brak czasu
  }
  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%Y-%m-%d_%H-%M-%S", &timeinfo);
  return String(timeStringBuff);
}

void setupOTA() {
  ArduinoOTA.setHostname("esp32-cam-trap");
  ArduinoOTA.begin();
  Serial.println("OTA gotowe.");
}

void handleOTA() {
  ArduinoOTA.handle();
}
