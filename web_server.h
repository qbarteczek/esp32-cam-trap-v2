#pragma once

#include <WebServer.h>
#include "FS.h"
#include "SD_MMC.h"
#include "network_ota.h"

extern WebServer server;
extern bool isWebServerMode;

void handleRoot() {
  String html = "<html><body><h1>ESP32-CAM V2 Portal</h1>";
  html += "<h2>Pliki na karcie SD:</h2><ul>";
  
  File root = SD_MMC.open("/");
  if(root){
    File file = root.openNextFile();
    while(file){
      if(!file.isDirectory()){
        html += "<li><a href=\"/download?f=" + String(file.name()) + "\">" + String(file.name()) + "</a></li>";
      }
      file = root.openNextFile();
    }
  }
  html += "</ul><hr>";
  
  html += "<h2>Ustawienia:</h2>";
  html += "<form action=\"/save\" method=\"POST\">";
  html += "SSID: <input type=\"text\" name=\"ssid\" value=\"" + ssid + "\"><br>";
  html += "Haslo: <input type=\"password\" name=\"pass\" value=\"" + password + "\"><br>";
  html += "Bot Token: <input type=\"text\" name=\"token\" value=\"" + botToken + "\"><br>";
  html += "Chat ID: <input type=\"text\" name=\"chat\" value=\"" + chatId + "\"><br>";
  html += "<input type=\"submit\" value=\"Zapisz i Restart\"></form>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleSave() {
  if (server.hasArg("ssid") && server.hasArg("pass") && server.hasArg("token") && server.hasArg("chat")) {
    savePreferences(server.arg("ssid"), server.arg("pass"), server.arg("token"), server.arg("chat"));
    server.send(200, "text/html", "Zapisano. Trwa restart...");
    delay(1000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Brak wymaganych pol.");
  }
}

void handleDownload() {
  if(server.hasArg("f")){
    String path = "/" + server.arg("f");
    File file = SD_MMC.open(path, FILE_READ);
    if(file){
      server.streamFile(file, "image/jpeg");
      file.close();
      return;
    }
  }
  server.send(404, "text/plain", "Plik nie istnieje.");
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/download", HTTP_GET, handleDownload);
  server.begin();
  Serial.println("Web Server uruchomiony na porcie 80.");
}

void startAPMode() {
  Serial.println("Uruchamiam Access Point do konfiguracji...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-CAM-Config", "12345678");
  Serial.println("AP IP: " + WiFi.softAPIP().toString());
  isWebServerMode = true;
  setupWebServer();
}
