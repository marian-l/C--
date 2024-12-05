#include <Arduino.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "vector"
#include <Wire.h>
#include <SPI.h>
#include "BH1750.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

// Marian WiFi
const char * ssid = "Vodafone-6874";
const char * password = "LTHrrxqXgrMr2q9G";

AsyncWebServer server(80);
AsyncWebSocket ws("/");

// State of action for the ESP32
enum state {
    SEND_ALL,
    SEND_PARTS,
    SLEEP,
    IDLE
};

state current_state = SEND_ALL;

// available sensors
constexpr const char *LIGHT_SENSOR = "LIGHT";
constexpr const char *PRESSURE_SENSOR = "AIR_PRESSURE";
constexpr const char *TEMPERATURE_SENSOR = "TEMPERATURE";
constexpr const char *HUMIDITY_SENSOR = "HUMIDITY";
constexpr const char *VOLUME_SENSOR = "VOLUME";

const std::vector<String> sensors = {HUMIDITY_SENSOR, LIGHT_SENSOR, PRESSURE_SENSOR, TEMPERATURE_SENSOR, VOLUME_SENSOR};
std::vector<String>  sending_sensors = {HUMIDITY_SENSOR, LIGHT_SENSOR, PRESSURE_SENSOR, TEMPERATURE_SENSOR, VOLUME_SENSOR};

BH1750 lightSensor;
Adafruit_BME280 bme280;

float temperature;
float humidity;
float pressure;
float lux;
float volume;

// get current state of the server (what is she/he doing)
void getStatus() {

}

// get the time interval of the server (how often is he/she doing)
void setInterval(uint8_t uint8) {

}

// get the server to rest
void handleSleepCommand(uint8_t uint8) {

}

// remove sensor from active usage (sending_sensors-list)
void handleRemoveSensor(uint8_t uint8) {

}

// this should listen to messages for the server
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    // not sure what this AwsFrameInfo is, probably asyncWebServer
    AwsFrameInfo *info = (AwsFrameInfo*)arg;

    if (len < 2) {
        Serial.printf("Invalid message length: %d\n", len);
        return;
    }

    // opcode implies that the websocket expects textual data (ASCII or UTF-8)
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0; // add null-terminator for data to be treated as string-array.

        // data is a pointer to uint8_t, where each byte corresponds to a ACSII-char
        char main_command = data[0];

        // shoudl print command
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

        // if (strcmp((char*)data, "Please, turn on the green LED.") == 0) {
        //     if (current_state == LED) { // Check current state
        //         current_state = TEMPERATURE; // Change current state
        //         Serial.printf("LED: Green\n");
        //         digitalWrite(RED_LED_PIN, LOW); // Turn off red the LED
        //         digitalWrite(GREEN_LED_PIN, HIGH); // Turn on green the LED
        //     }
        // }
        // else if (strcmp((char*)data, "Please, turn on the red LED.") == 0) {
        //     if (current_state == LED) { // Check current state
        //         current_state = TEMPERATURE; // Change current state
        //         Serial.printf("LED: Red\n");
        //         digitalWrite(GREEN_LED_PIN, LOW); // Turn off green the LED
        //         digitalWrite(RED_LED_PIN, HIGH); // Turn on red the LED
        //     }
        // }
        // else {
        //     current_state = TEMPERATURE; // Reset state
        // }
    }
}

// specific events the server goes through
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            // Print connected client info
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            // Print disconnecter client info
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            // print event info
            Serial.printf("Client #%u sent data: %.*s (length: %d)\n", client->id(), len, data, len);

            // Handle received data
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            Serial.printf("WebSocket error for client #%u\n", client->id());
            break;
    }
}

void setup() {
    Serial.begin(115200); // Start ESP32 serial communication

    // initialize I2C Bus (BH1750 lib does not do it automatically
    Wire.begin(); // Wire.begin(SDA, SCL)

    // Byte adresse und i2c Bus können auch versorgt werden
    lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

    // BME-Sensor
    if(!bme280.begin(0x76)) {
        Serial.println("BME280 konnte nicht gefunden werden!");
    }

    // esp as a variable has a lot of members to look at.

    // Start ESP32 Access Point mode (ssid password channels ssid_hidden max_connection)
    WiFi.softAP("ambient_disco_ap", "rocking_stone", 1, 0, 4);
    Serial.println(WiFi.softAPIP()); // Print the websocket server IP to connect

    ws.onEvent(onEvent); // Start listening for events on the websocket server
    server.addHandler(&ws); // Add the websocket server handler to the webserver

    server.begin(); // Start listening for socket connections
}

void loop() {
    // wie viel Speicher ist noch da?
    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());

    // Lichtmessung
    float lux = lightSensor.readLightLevel();
    Serial.printf("Light level: %f lx \n", lux);

    // Temperatur lesen
    temperature = bme280.readTemperature();
    //temp = 1.8*bme280.readTemperature() + 32;

    // Luftfeuchtigkeit lesen
    humidity = bme280.readHumidity();

    // Luftdruck lesen
    pressure = bme280.readPressure()/100.0F;

    // machen, dass der ESP32 chillt
    delay(1000);
    ws.cleanupClients();
}