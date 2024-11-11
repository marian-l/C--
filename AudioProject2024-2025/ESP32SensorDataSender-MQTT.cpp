//
// Created by maria on 02.11.2024.
//
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";
const char* mqtt_server = "192.168.1.100";  // Replace with your computer's IP address

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
    delay(10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void reconnect() {
    while (!client.connected()) {
        client.connect("ESP32Client");
    }
}

void setup() {
    setup_wifi();
    client.setServer(mqtt_server, 1883);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    float temperature = 22.5; // Example temperature value; replace with actual sensor reading
    String payload = String(temperature);
    client.publish("weather/temperature", payload.c_str());

    delay(1000); // Send data every second
}