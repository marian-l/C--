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

// STATES
enum state {SEND_ALL, SEND_PARTS, SLEEP, IDLE };
state current_state = SEND_ALL;

// Sensors
constexpr const char *LIGHT_SENSOR = "LIGHT";
constexpr const char *PRESSURE_SENSOR = "AIR_PRESSURE";
constexpr const char *TEMPERATURE_SENSOR = "TEMPERATURE";
constexpr const char *HUMIDITY_SENSOR = "HUMIDITY";
constexpr const char *VOLUME_SENSOR = "VOLUME";

const std::vector<String> sensors = {HUMIDITY_SENSOR, LIGHT_SENSOR, PRESSURE_SENSOR, TEMPERATURE_SENSOR, VOLUME_SENSOR};
std::vector<String>  sending_sensors = {HUMIDITY_SENSOR, LIGHT_SENSOR, PRESSURE_SENSOR, TEMPERATURE_SENSOR, VOLUME_SENSOR};

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
    } else if (type == WS_EVT_DATA) {
        // commands are not needed as the http request will always just ask for the page.
        // onWebSocketMessage(arg, data, len);
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

    // BME-Sensor
    // if(!bme280.init()) {
    //     Serial.println("BME280 konnte nicht gefunden werden!");
    // }

    pinMode(digitalPin, INPUT);

    // Server
    server.serveStatic("/", LittleFS, "/").setDefaultFile("test-website.html");
    // server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    server.onNotFound(handleNotFound);
    server.addHandler(&ws);
    server.begin();

    // Websocket Event Handler
    ws.onEvent(onWebSocketEvent);
}

void notifyClients() {
    String jsonData = "{\"temperature\": " + String(temperature) +
                      ", \"humidity\": " + String(humidity) +
                      ", \"light\": " + String(lux) +
                      ", \"pressure\": " + String(pressure) +
                      ", \"volume\": " + String(sound) + "}";

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

        sound = analogRead(analogPin); // Read the analog value
        // int digitalValue = digitalRead(digitalPin); // Read the digital value
        Serial.print("Analog Value: ");
        Serial.print(sound);
        // Serial.print(" | Digital Value: ");
        // Serial.println(digitalValue);

        notifyClients();

        delay(1000);
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

void handleRemoveSensor(uint8_t data) {

}

void handleSleepCommand(uint8_t data) {

}

void setInterval(uint8_t data) {

}

void getStatus() {

}

void onWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;

    if (len < 2) {
        Serial.printf("Invalid message length: %d\n", len);
        ws.textAll("Command with invalid length received.");
        return;
    }

    // opcode implies that the websocket expects textual data (ASCII or UTF-8)
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0; // add null-terminator for data to be treated as string-array.

        // data is a pointer to uint8_t, where each byte corresponds to a ACSII-char
        char main_command = data[0];

        Serial.printf("Received command: %c\n", data[0]);

        // ggf. handleCommand hier implementieren
        switch (main_command) {
            case 'r':
                Serial.printf("Remove sensor: %c\n", data[1]);
                handleRemoveSensor(data[1]); // pass the sensor to be removed
                break;
            case 's':
                Serial.printf("Sleep for: %d\n", data[1]);
                handleSleepCommand(data[1]); // minutes of sleep (1 byte -> 15min)
                break;
            case 'i':
                Serial.printf("Setting transmission interval to: every %d minutes", data[1]);
                setInterval(data[1]);
                break;
            case 'z':
                getStatus(); // z für Zustand
                break;
            default:
                Serial.printf("Unrecognized command: %c \n", main_command);
                break;
        }
    }
}