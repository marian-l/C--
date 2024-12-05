#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include <LittleFS.h>
#include <SPI.h>

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
    server.addHandler(&ws);

    server.begin();
}

void setupWiFiAP() {
    WiFi.softAP(ssid, password);
}

void onWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        // Echo the message back to the client
        ws.textAll((char*)data);
    }
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    } else if (type == WS_EVT_DATA) {
        onWebSocketMessage(arg, data, len);
    }
}

void setup() {
    Serial.begin(115200);

    setupWiFiAP();

    if (!LittleFS.begin()) {
        Serial.println("Fehler beim Mounten von LittleFS");
        return;
    }

    setupWebServer();

    ws.onEvent(onWebSocketEvent);
}

void notifyClients() {
    if (ws.count() > 0) {
        String jsonData = "{\"temperature\": " + String(random(10, 100) / 10.0) +
                          ", \"humidity\": " + String(random(10, 100) / 10.0) +
                          ", \"light\": " + String(random(10, 100) / 10.0) + "}";

        ws.textAll(jsonData); // Send data to all connected WebSocket clients
    }
}

void loop() {
    delay(5000);
    Serial.println("Beep beep booop.");

    notifyClients();
}

int main() {
    setup();
    while (true) {
        loop();
    }
}