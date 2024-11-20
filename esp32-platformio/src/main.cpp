#include <Arduino.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include "vector"
#include <Wire.h>
#include <BH1750.h>
#include <SPI.h>

// #include <DHT.h> // This is a dependency of DHT22 temperature and humidity sensor used in the git

// #define DHT_PIN 4 // this is the pin of the users sensor
// #define DHT_TYPE DHT11 // this is the version of the sensor

// DHT dht(DHT_PIN, DHT_TYPE);

AsyncWebServer server(80);
AsyncWebSocket ws("/");

const char *ssid = "ESP32_MHML";
const char *password = "password";

enum state {
    SEND_ALL,
    SEND_PARTS,
    SLEEP,
    IDLE
};

state current_state = SEND_ALL;

constexpr const char *LIGHT_SENSOR = "LIGHT";
constexpr const char *PRESSURE_SENSOR = "AIR_PRESSURE";
constexpr const char *TEMPERATURE_SENSOR = "TEMPERATURE";
constexpr const char *HUMIDITY_SENSOR = "HUMIDITY";

const std::vector<String> sensors = {HUMIDITY_SENSOR, LIGHT_SENSOR, PRESSURE_SENSOR, TEMPERATURE_SENSOR};
std::vector<String>  sending_sensors = {HUMIDITY_SENSOR, LIGHT_SENSOR, PRESSURE_SENSOR, TEMPERATURE_SENSOR};

BH1750 lightSensor;

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

    // esp as a variable has a lot of members to look at.

    WiFi.softAP(ssid, password); // Start ESP32 Access Point mode
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

    // machen, dass der ESP32 chillt
    delay(1000);
    ws.cleanupClients();
}

int main() {
    std::printf("testing..");
    return 0;
}