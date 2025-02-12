#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include <LittleFS.h>
#include <SPI.h>

// sensors
#include "BH1750.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

#define BH1750_ADDRESS 0x23

// Server Socket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// WiFi
const char* ssid =     "ambient_disco_ap";
const char* password = "rocking_stone";

BH1750 lightSensor;
Adafruit_BME280 bme280;

// Sensor Values
float temperature;
float humidity;
float pressure;
float lux;
int sound; // sound

const int analogPin = A0; // Analog output from the KY-038
const int digitalPin = 35; // Digital output from the KY-038

void handleNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "There is nothing here for you to see");
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    }
}

void setup() {
    Serial.begin(115200);

    // Wifi Access Point
    WiFi.softAP(ssid, password);

    // File System
    if (!LittleFS.begin()) {
        Serial.println("Fehler beim Mounten von LittleFS");
        return;
    }

    if(!Wire.begin()) {
        Serial.println("Fehler beim Starten des Wire");
    }

    if(!lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C)) {
        Serial.printf("Fehler beim Starten des Lichtsensors");
    }

    pinMode(digitalPin, INPUT);

    // Server
    // server.serveStatic("/", LittleFS, "/").setDefaultFile("test-website.html");
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    server.onNotFound(handleNotFound);
    server.addHandler(&ws);
    server.begin();

    // Websocket Event Handler
    ws.onEvent(onWebSocketEvent);
}

void notifyClients() {
    String jsonData = "{\"temperature\": " + String(temperature) +
                      ",\n" + " \"humidity\": " + String(humidity) +
                      ",\n" + " \"brightness\": " + String(lux) +
                      ",\n" + " \"pressure\": " + String(pressure) +
                      ",\n" + " \"noise\": " + String(sound) + "}";

    ws.textAll(jsonData); // Send data to all connected WebSocket clients
}

void loop() {
    Serial.printf("WS-Connections: %d", ws.count());

    if (ws.count() > 0) {
        // wie viel Speicher ist noch da?
        Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());

        if (lightSensor.measurementReady()) {
            lux = lightSensor.readLightLevel();
            Serial.printf("lux: %f ", lux);
        }

        if (!bme280.begin(0x76)) {
            Serial.println("BME280 is not running.");
        } else {
            temperature = bme280.readTemperature();
            humidity = bme280.readHumidity();
            pressure = bme280.readPressure()/100.0F;

            Serial.printf("temperature: %f ", temperature);
            Serial.printf("humidity: %f ", humidity);
            Serial.printf("pressure: %f ", pressure);
        }
        auto test = digitalRead(digitalPin);
        Serial.printf("\n Digital Value: %i", test);

        sound = analogRead(analogPin); // Read the analog value
        Serial.printf("Analog Value: %i", sound);

        notifyClients();

        delay(200);
    } else {
        delay(5000);
    }

    Serial.println("Beep beep booop.");
}

int main() {
    setup();
    while (true) {
        loop();
    }
}