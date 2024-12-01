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
    esp_err_t result = esp_wifi_init(&config);
    if (result != ESP_OK) {
        Serial.printf("WiFi init failed: %d\n", result);
        return;
    }

    // Explicitly initialize TCP/IP stack
    result = esp_netif_init();
    if (result != ESP_OK) {
        Serial.println("Network Interface Initiation failed");
    }
    Serial.printf("WiFi init result: %d\n", result);

    esp_netif_t *netif_ap = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    if (netif_ap) {
        Serial.println("Existing AP netif detected. Destroying...");
        esp_netif_destroy(netif_ap);
    }

    // get pointer the network interface instance
    netif_ap = esp_netif_create_default_wifi_ap();
    Serial.println("Creating netif_ap");
    // esp_netif_obj* netif_ap = esp_netif_create_default_wifi_ap();

    result = esp_wifi_set_mode(WIFI_MODE_AP);
    Serial.printf("WiFi-Mode: %d\n", result);

    wifi_config_t apConfig = {};

    if (!result) {
        strcpy((char *) apConfig.ap.ssid, "ambient_disco_ap");
        strcpy((char *) apConfig.ap.password, "rocking_stone");
        apConfig.ap.channel = 1;
        apConfig.ap.max_connection = 4;
        // apConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
        apConfig.ap.authmode = WIFI_AUTH_OPEN;
    }

    result = esp_wifi_set_config(WIFI_IF_AP, &apConfig);
    if (result != ESP_OK) {
        Serial.printf("Failed to configure AP: %d\n", result);
        return;
    }

    result = esp_wifi_start();
    if (result != ESP_OK) {
        Serial.printf("Failed to start WiFi: %d\n", result);
        return;
    }

    // auto handle = esp_netif_get_handle_from_ifkey(netif_ap);

    // Retrieve IP address of the AP
    esp_netif_ip_info_t ip_info;
    result = esp_netif_get_ip_info(netif_ap, &ip_info);
    if (result == ESP_OK) {
        Serial.printf("AP IP Address: %s\n", ip4addr_ntoa(reinterpret_cast<const ip4_addr_t *>(&ip_info.ip)));
    } else {
        Serial.printf("Failed to retrieve IP address: %s\n", esp_err_to_name(result));
    }

    // configure DHCP
    IPAddress local_IP(192, 168, 4, 1);    // Default AP IP address
    IPAddress gateway(192, 168, 4, 1);    // Gateway IP
    IPAddress subnet(255, 255, 255, 0);   // Subnet mask

    result = esp_netif_dhcpc_start(netif_ap);
    Serial.printf("DHCP-Status code: %d\n", result);

    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
}

void setup_wifi_alt() {
    return;
    // if (result != ESP_OK) {
    //     result = esp_wifi_set_mode(WIFI_AP);
    //     Serial.printf("WiFi-Mode: %d\n", result);
    // }

    // if (result != ESP_OK) {
    // Serial.printf("Failed to use ESP-method to set WiFi-mode: %s\n", esp_err_to_name(result));
    // Serial.printf("Failed to use ESP-method to set WiFi-mode: %d\n", result);
    // return;
    // }

    // both give true, should be working?
    // String error = WiFi.softAPtest("ambient_disco_ap", NULL, 1, 0, 4, 0);
    // Serial.printf("softApTestResult: %s\n", error.c_str());
//
    // String error = WiFi.softAPtest("ambient_disco_ap", NULL, 1, 0, 4, 1);
    // Serial.printf("softApTestResult with FTM: %s\n", error.c_str());

    // Start ESP32 Access Point mode (ssid password channels ssid_hidden max_connection)
    // result = WiFi.softAP("ambient_disco_ap", nullptr, 1, 0, 4, 1);
    // Serial.printf("Access-Point setup: %d\n", result);



    // Start ESP32 Access Point mode (ssid password channels ssid_hidden max_connection)
    // if (!WiFi.softAP("ambient_disco_ap", "rocking_stone", 1, 0, 4)) {
    //     Serial.println("Failed to start Access Point!");
//
    //     result = false;
    // }

    // Serial.printf("Access Point IP: %s\n", WiFi.softAPIP().toString().c_str());

    // Serial.println("Access Point started successfully.");
    // Serial.print("AP IP Address: ");
    // Serial.println(WiFi.softAPIP());

    // IPAddress local_IP(192, 168, 1, 64);    // Set a static IP
    // IPAddress gateway(192, 168, 1, 1);    // Gateway IP
    // IPAddress subnet(255, 255, 255, 0);   // Subnet mask

    // if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    //     Serial.println("Failed to configure AP network settings");
    // }

    // wl_status_t wifi_result = WiFi.begin();
    // Serial.printf("WiFi-Result (.begin()): %d\n", wifi_result);
}

void wifiEventHandler(WiFiEvent_t event, WiFiEventInfo_t info) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("ARDUINO_EVENT_WIFI_AP_START");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Serial.println("ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.printf("Device connected: %02X:%02X:%02X:%02X:%02X:%02X\n",
                          info.wifi_ap_staconnected.mac[0], info.wifi_ap_staconnected.mac[1], info.wifi_ap_staconnected.mac[2],
                          info.wifi_ap_staconnected.mac[3], info.wifi_ap_staconnected.mac[4], info.wifi_ap_staconnected.mac[5]);
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.printf("Device disconnected: %02X:%02X:%02X:%02X:%02X:%02X\n",
                          info.wifi_ap_stadisconnected.mac[0], info.wifi_ap_stadisconnected.mac[1], info.wifi_ap_stadisconnected.mac[2],
                          info.wifi_ap_stadisconnected.mac[3], info.wifi_ap_stadisconnected.mac[4], info.wifi_ap_stadisconnected.mac[5]);
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200); // Start ESP32 serial communication

    // esp as a variable has a lot of members to look at.

    setup_wifi();

    // WiFi Event Handler
    WiFi.onEvent(wifiEventHandler);

    // Start listening for events on the websocket server
    ws.onEvent(onEvent);

    // Add the websocket server handler to the webserver
    server.addHandler(&ws);

    // Start listening for socket connections
    server.begin();

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