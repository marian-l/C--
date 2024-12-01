#include <Arduino.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "vector"
#include <Wire.h>
#include "BH1750.h"
#include <SPI.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "esp_wifi.h"

// Marian WiFi 
// const char * ssid = "Vodafone-6874";
// const char * password = "LTHrrxqXgrMr2q9G";

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
                break;
            case 's':
                Serial.printf("Sleep for: %d\n", data[1]);
                break;
            case 'i':
                Serial.printf("Setting transmission interval to: every %d minutes", data[1]);
                break;
            case 'z':
                break;
            default:
                Serial.printf("Unrecognized command: %c \n", main_command);
                break;
        }
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

void setup_wifi() {
    // WiFi config
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();

    // Explicitly initialize the WiFi driver
    esp_err_t init_result = esp_wifi_init(&config);
    if (init_result != ESP_OK) {
        Serial.printf("WiFi init failed: %d\n", init_result);
        Serial.printf("WiFi-init err name: %s\n", esp_err_to_name(init_result));
        return;
    }

    Serial.printf("WiFi init result: %d\n", init_result);

    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_OFF);
    // delay(100);

    // int WiFiStatus = WiFi.begin("ambient_disco_ap", "rocking_stone", 1);
    // Serial.printf("WiFiStatus %d\n", WiFiStatus);

    bool result = WiFi.mode(WIFI_AP);
    Serial.printf("Wifi-Mode: %d\n", result);

    esp_err_t esp_result;

    if (!result) {
        esp_result = esp_wifi_set_mode(WIFI_MODE_AP);
    }

    if (esp_result != ESP_OK) {
        Serial.printf("Failed to use ESP-method to set WiFi-mode: %s\n", esp_err_to_name(esp_result));
        Serial.printf("Failed to use ESP-method to set WiFi-mode: %d\n", esp_result);
        return;
    }

    // Start ESP32 Access Point mode (ssid password channels ssid_hidden max_connection)
    if (!WiFi.softAP("ambient_disco_ap", "rocking_stone", 1, 0, 4)) {
        Serial.println("Failed to start Access Point!");

        result = false;
    }

    wifi_config_t apConfig = {};

    if (!result) {
        strcpy((char *) apConfig.ap.ssid, "ambient_disco_ap");
        strcpy((char *) apConfig.ap.password, "rocking_stone");
        apConfig.ap.channel = 1;
        apConfig.ap.max_connection = 4;
        apConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    }

    esp_result = esp_wifi_set_config(WIFI_IF_AP, &apConfig);
    if (esp_result != ESP_OK) {
        Serial.printf("Failed to configure AP: %s\n", esp_err_to_name(result));
        Serial.printf("Failed to configure AP: %d\n", esp_result);
        return;
    }

    result = esp_wifi_start();
    if (result != ESP_OK) {
        Serial.printf("Failed to start WiFi: %s\n", esp_err_to_name(result));
        Serial.printf("Failed to start WiFi: %d\n", esp_result);
        return;
    }

    // Start ESP32 Access Point mode (ssid password channels ssid_hidden max_connection)
    if (!WiFi.softAP("ambient_disco_ap", "rocking_stone", 1, 0, 4)) {
        Serial.println("Failed to start Access Point!");

        result = false;
    }

    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());

    Serial.printf("Access Point IP: %s\n", WiFi.softAPIP().toString().c_str());

    // if softAPIP fails
    if (WiFi.softAPIP() == reinterpret_cast<const uint8_t *>("0.0.0.0")) {
        tcpip_adapter_ip_info_t ip_info;
        if (tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip_info) == ESP_OK) {
            Serial.printf("AP IP Address: %s\n", ip4addr_ntoa(&ip_info.ip));
        } else {
            Serial.println("Failed to retrieve IP address.");
        }
    }
}

void setup() {
    Serial.begin(115200); // Start ESP32 serial communication

    // esp as a variable has a lot of members to look at.

    setup_wifi();

    // Serve static HTML/JS files
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta content="width=device-width, initial-scale=1.0" name="viewport">
        <title>Web Audio API Demo</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                text-align: center;
                padding: 2em;
            }

            input[type="range"] {
                width: 300px;
                margin: 1em;
            }
        </style>
    </head>
    <body>
        <h1>Web Audio API Demo</h1>
        <p>Use the sliders below to adjust the frequency and volume of the tone.</p>

        <div>
            <label for="frequency">Frequency (Hz): <span id="freq-value">440</span></label><br>
            <input id="frequency" max="2000" min="100" type="range" value="440">
        </div>

        <div>
            <label for="volume">Volume: <span id="vol-value">50</span></label><br>
            <input id="volume" max="100" min="0" type="range" value="50">
        </div>

        <button id="start">Start Tone</button>
        <button id="stop">Stop Tone</button>

        <script>
            const ws = new WebSocket("ws://" + location.host + "/");

            ws.onopen = () => {
                console.log("WebSocket connected");
            }

            ws.onmessage = (event) => {
                const sensorData = JSON.parse(event.data);
                console.log("Received following data: ", sensorData);

                // first only the single oscillator
                if (oscillator) {
                    const frequency = 200 + sensorData.temperature * 10;
                    const volume = sensorData.humidity / 100;
                    const light = sensorData.light / 100;

                    oscillator.frequency.setValueAtTime(frequency, audioContext.currentTime);
                    gainNode.gain.setValueAtTime(volume, audioContext.currentTime);

                    // fun effects
                    modulationNode.gain.setValueAtTime(light, audioContext.currentTime);
                }
            }

            let audioContext = null;
            let oscillator = null;
            let gainNode = null;

            // Initialize Audio Context
            function initAudio() {
                if (!audioContext) {
                    audioContext = new (window.AudioContext || window.webkitAudioContext)();
                    oscillator = audioContext.createOscillator();
                    gainNode = audioContext.createGain();
                    modulationNode = audioContext.createGain();
                    modulationOscilattor = audioContext.createOscillator();

                    oscillator.type = 'sine'; // Type of wave: sine, square, sawtooth, triangle
                    oscillator.frequency.setValueAtTime(440, audioContext.currentTime); // Default frequency: 440Hz
                    gainNode.gain.setValueAtTime(0.5, audioContext.currentTime); // Default volume: 50%

                    // Connect nodes: Oscillator -> Gain -> Destination
                    oscillator.connect(gainNode);
                    gainNode.connect(audioContext.destination);

                    oscillator.start(); // Start the oscillator immediately
                    gainNode.gain.setValueAtTime(0, audioContext.currentTime); // Mute initially
                }
            }

            // Start the tone
            document.getElementById('start').addEventListener('click', () => {
                initAudio();
                gainNode.gain.setValueAtTime(
                    document.getElementById('volume').value / 100,
                    audioContext.currentTime
                );
            });

            // Stop the tone
            document.getElementById('stop').addEventListener('click', () => {
                if (audioContext) {
                    gainNode.gain.setValueAtTime(0, audioContext.currentTime); // Mute the sound
                }
            });

            // Adjust frequency
            document.getElementById('frequency').addEventListener('input', (event) => {
                const frequency = event.target.value;
                document.getElementById('freq-value').textContent = frequency;
                if (oscillator) {
                    oscillator.frequency.setValueAtTime(frequency, audioContext.currentTime);
                }
            });

            // Adjust volume
            document.getElementById('volume').addEventListener('input', (event) => {
                const volume = event.target.value;
                document.getElementById('vol-value').textContent = volume;
                if (gainNode) {
                    gainNode.gain.setValueAtTime(volume / 100, audioContext.currentTime);
                }
            });
        </script>
    </body>
</html>

    )rawliteral");
    });

    ws.onEvent(onEvent); // Start listening for events on the websocket server
    server.addHandler(&ws); // Add the websocket server handler to the webserver

    server.begin(); // Start listening for socket connections
}

void notifyClients() {
    if (ws.count() > 0) {
        String jsonData = "{\"temperature\": " + String(random(10, 100) / 10.0) +
                          ", \"humidity\": " + String(random(10, 100) / 10.0) +
                          ", \"light\": " + String(random(10, 100) / 10.0) + "}";

        ws.textAll(jsonData); // Send data to all connected WebSocket clients
    }
}
int i = 0;

void loop() {
    // wie viel Speicher ist noch da?

    if (i == 0) {
        setup_wifi();

        i = 1;
    }


    ws.cleanupClients();

    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 100) { // Send data every 0.1 seconds
        notifyClients();
        lastSendTime = millis();
    }
}