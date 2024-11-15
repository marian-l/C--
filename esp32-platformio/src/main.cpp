#include <Arduino.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include <cstdint>
#include "string"
#include "list"

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

const std::string light_sensor = "LIGHT";
const std::string pressure_sensor = "AIR_PRESSURE";
const std::string temperature_sensor = "TEMPERATURE";
const std::string humidity_sensor = "HUMIDITY";

const std::list<std::string> sensors = {humidity_sensor, light_sensor, pressure_sensor, temperature_sensor};
std::list<std::string> sending_sensors = {humidity_sensor, light_sensor, pressure_sensor, temperature_sensor};

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

    // what are these tests?
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;

        // alternative Kommandos werden gebraucht.
        // Eventuell Konfiguration der Sendeintervalle, Sleep-Modus und Ansteuern einzelner Sensoren, Status
        char main_command = data[0];
        switch (main_command) {
            case 'r':
                handleRemoveSensor(data[1]); // pass the sensor to be removed
            case 's':
                handleSleepCommand(data[1]); // minutes of sleep
            case 'i':
                setInterval(data[1]);
            case 'z':
                getStatus(); // z für Zustand
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
            // Handle received data
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void setup() {
    Serial.begin(115200); // Start ESP32 serial communication

    dht.begin(); // Start DHT with initial parameters

    pinMode(GREEN_LED_PIN, OUTPUT); // Set the green LED pin as output
    pinMode(RED_LED_PIN, OUTPUT); // Set the red LED pin as output

    WiFi.softAP(ssid, password); // Start ESP32 Access Point mode
    Serial.println(WiFi.softAPIP()); // Print the websocket server IP to connect

    ws.onEvent(onEvent); // Start listening for events on the websocket server
    server.addHandler(&ws); // Add the websocket server handler to the webserver

    server.begin(); // Start listening for socket connections
}

void loop() {
    ws.cleanupClients();
}

int main() {
    std::printf("testing..");
    return 0;
}