#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include <LittleFS.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char* ssid =     "ambient_disco_ap";
const char* password = "rocking_stone";

void handleNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void handleRoot(AsyncWebServerRequest* request) {
    request->send(LittleFS, "/test-website.html", "text/html");
}

void setupWebServer() {
    server.serveStatic("/", LittleFS, "/").setDefaultFile("test-website.html");
    server.onNotFound(handleNotFound);
    server.begin();
}

void setupWiFiAP() {
    WiFi.softAP(ssid, password);
}

void setup() {
    Serial.begin(115200);

    setupWiFiAP();

    if (!LittleFS.begin()) {
        Serial.println("Fehler beim Mounten von LittleFS");
        return;
    }

    setupWebServer();
}

void loop() {
    delay(5000);
    Serial.println("Beep beep booop.");


}

int main() {
    setup();
    while (true) {
        loop();
    }
}