#include <WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";
WebSocketsClient webSocket;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    webSocket.begin("192.168.1.100", 8765, "/");  // IP and port of WebSocket server
}

void loop() {
    webSocket.loop();
    String data = "{\"temperature\": 24.3, \"humidity\": 60}";
    webSocket.sendTXT(data);  // Send data as a text message
    delay(1000);  // Send every second
}//
// Created by maria on 06.11.2024.
//
