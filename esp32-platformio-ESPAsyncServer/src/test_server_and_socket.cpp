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

AsyncWebServer *as_server_pt;
AsyncWebSocket *ws_pt;

// AsyncWebSocket *ws = new AsyncWebSocket("/");
// std::unique_ptr<AsyncWebSocket> ws(new AsyncWebSocket("/ws"));
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
void onEvent(AsyncWebSocket *server,
             AsyncWebSocketClient *client,
             AwsEventType type,
             void *arg,
             uint8_t *data,
             size_t len) {
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

// Event handler function
void WiFiEventHandler(arduino_event_id_t event, arduino_event_info_t info) {
    Serial.println("Called WiFi Event Handler");
    switch (event) {
            // Wi-Fi AP Events
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("WiFi AP Started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("WiFi AP Stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.printf("WiFi AP: Station Connected - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                          info.wifi_ap_staconnected.mac[0],
                          info.wifi_ap_staconnected.mac[1],
                          info.wifi_ap_staconnected.mac[2],
                          info.wifi_ap_staconnected.mac[3],
                          info.wifi_ap_staconnected.mac[4],
                          info.wifi_ap_staconnected.mac[5]);
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.printf("WiFi AP: Station Disconnected - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                          info.wifi_ap_stadisconnected.mac[0],
                          info.wifi_ap_stadisconnected.mac[1],
                          info.wifi_ap_stadisconnected.mac[2],
                          info.wifi_ap_stadisconnected.mac[3],
                          info.wifi_ap_stadisconnected.mac[4],
                          info.wifi_ap_stadisconnected.mac[5]);
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Serial.printf("WiFi AP: Probe Request Received - RSSI: %d\n", info.wifi_ap_probereqrecved.rssi);
            break;

            // Wi-Fi Event Errors
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Serial.println("WiFi WPS: Success");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Serial.println("WiFi WPS: Failed");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Serial.println("WiFi WPS: Timeout");
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Serial.printf("WiFi WPS: PIN - %s\n", info.wps_er_pin.pin_code);
            break;

            // Default Event
        default:
            Serial.printf("Unhandled WiFi Event: %d\n", event);
            break;
    }
}

void setup() {
    // esp as a variable has a lot of members to look at.
    // increase log level for debugging purposes
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    Serial.begin(115200); // Start ESP32 serial communication

    // TCP/IP stack
    esp_err_t result = esp_netif_init();
    if (result != ESP_OK) {
        Serial.println("Network Interface Initiation failed");
    }
    Serial.printf("Network Interface Init result: %d\n", result);

    // WiFi Driver
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    result = esp_wifi_init(&config);
    if (result != ESP_OK) {
        Serial.printf("WiFi init failed: %d\n", result);
        return;
    }
    Serial.printf("WiFi-Driver Init result: %d\n", result);

    // Network Interface
    esp_netif_t *netif_ap = esp_netif_create_default_wifi_ap();
    if (!netif_ap) {
        Serial.println("Network Interface AP Creation failed");
    } // attach network interface for ap mode
    result = esp_netif_attach_wifi_ap(netif_ap);
    if (result != ESP_OK) {
        Serial.printf("WiFi-AP and Network Interface could not be connected: %d\n", result);
    }
    // manual creation
    // esp_netif_t *netif_ap = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
    // result = esp_netif_attach_wifi_ap(netif_ap);

    // Check WiFi-Mode
    wifi_mode_t wifi_mode;
    result = esp_wifi_get_mode(&wifi_mode);
    Serial.printf("Get WiFi-Mode: %d\n", result);
    if (wifi_mode != WIFI_MODE_AP) {
        result = esp_wifi_set_mode(WIFI_MODE_AP);
        Serial.printf("Changing WiFi-Mode: %d\n", result);

        result = esp_wifi_get_mode(&wifi_mode);
        Serial.printf("Get WiFi-Mode: %d\n", result);
    }

    // Access-Point Configuration
    wifi_config_t apConfig = {};
    strcpy((char *) apConfig.ap.ssid, "ambient_disco_ap");
    strcpy((char *) apConfig.ap.password, "rocking_stone");
    apConfig.ap.channel = 1;
    apConfig.ap.max_connection = 4;// apConfig.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    // Setter for Configuration
    result = esp_wifi_set_config(WIFI_IF_AP, &apConfig);
    apConfig.ap.authmode = WIFI_AUTH_OPEN;
    if (result != ESP_OK) {
        Serial.printf("Failed to configure AP: %d\n", result);
        return;
    }
    Serial.printf("WiFi Configuration result: %d\n", result);

    // stop DHCP-Client and Server (just in case they are up) to configure the DHCP-Server
    result = esp_netif_dhcpc_stop(netif_ap); // Stop DHCP client (if running)
    if (result == ESP_ERR_ESP_NETIF_DHCP_NOT_STOPPED) {
        Serial.printf("DHCP Client was already stopped.");
    }
    else if (result != ESP_OK) {
        Serial.printf("Failed to stop DHCP client: %s\n", esp_err_to_name(result));
    }
    result = esp_netif_dhcps_stop(netif_ap); // Stop DHCP server
    if (result != ESP_ERR_ESP_NETIF_DHCP_ALREADY_STOPPED && result != ESP_OK) {
        Serial.printf("Failed to stop DHCP server: %s\n", esp_err_to_name(result));
    }

    result = esp_netif_is_netif_up(netif_ap);
    Serial.printf("Network Interface is (0=up, 1=down): %d\n", result);

    // configure DHCP
    esp_netif_ip_info_t ip_info;
    ip_info.ip.addr = ESP_IP4TOADDR(192, 168, 4, 1); // Local IP
    ip_info.gw.addr = ESP_IP4TOADDR(192, 168, 4, 1); // Gateway
    ip_info.netmask.addr = ESP_IP4TOADDR(255, 255, 255, 0); // Subnet mask
    result = esp_netif_set_ip_info(netif_ap, &ip_info);
    if (result != ESP_OK) {
        Serial.printf("Failed to set IP info: %s\n", esp_err_to_name(result));
        return;
    }
    Serial.println("Static IP set.");

    // start DHCP Server
    result = esp_netif_dhcps_start(netif_ap);
    Serial.printf("DHCP-Server-Status code: %d\n", result);

    // check Server Status
    esp_netif_dhcp_status_t dhcps_status;
    result = esp_netif_dhcps_get_status(netif_ap, &dhcps_status);
    Serial.printf("DHCP-Result after getter: %d\n", result);
    Serial.printf("DHCP-Status after getter: %d\n", dhcps_status);

    // WiFi Event Handler
    wifi_event_id_t wifi_event = WiFi.onEvent(WiFiEventHandler);
    Serial.printf("Amount of WiFi Event Handlers registered: %d\n", wifi_event);

    // Start WiFi
    result = esp_wifi_start();
    if (result != ESP_OK) {
        Serial.printf("Failed to start WiFi: %d\n", result);
        return;
    }

    // Retrieve IP address of the AP
    result = esp_netif_get_ip_info(netif_ap, &ip_info);
    if (result == ESP_OK) {
        Serial.printf("AP IP Address: %s\n", ip4addr_ntoa(reinterpret_cast<const ip4_addr_t *>(&ip_info.ip)));
    } else {
        Serial.printf("Failed to retrieve IP address: %s\n", esp_err_to_name(result));
    }

    // wie viel Speicher ist noch da?
    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());

    // let everything settle
    delay(1000);

    // create server and socket
    static AsyncWebServer as_server(80);
    delay(1000);
    static AsyncWebSocket ws("/ws");
    delay(1000);

    // debug server and socket ---> printing the address of the server freezes the program
    Serial.printf("Address of as_server: %p\n", &as_server);
    Serial.printf("Address of ws: %p\n", &ws);

    as_server_pt = &as_server;
    ws_pt = &ws;

    if (as_server_pt == nullptr || ws_pt == nullptr) {
        Serial.println("Failed to initialize server or WebSocket!");
        return;
    }

    // Start listening for events on the websocket server
    Serial.println("ws.onEvent()");
    ws.onEvent(onEvent);

    // Add the websocket server handler to the webserver
    Serial.println("server.addHandler()");
    as_server.addHandler(&ws);

    // Start listening for socket connections
    Serial.println("Server.begin()");
    as_server.begin();

    // wie viel Speicher ist noch da?
    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());

    // Serve static HTML/JS files
    as_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
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
                        pWiFiEventHandler         }

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
                    const ws_js = new WebSocket("ws_js://" + location.host + "/");

                    ws_js.onopen = () => {
                        console.log("WebSocket connected");
                    }

                    ws_js.onmessage = (event) => {
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
    Serial.println("Finished setup.");
}

void notifyClients() {
    if (ws_pt->count() > 0) {
        String jsonData = "{\"temperature\": " + String(random(10, 100) / 10.0) +
                          ", \"humidity\": " + String(random(10, 100) / 10.0) +
                          ", \"light\": " + String(random(10, 100) / 10.0) + "}";

        ws_pt->textAll(jsonData); // Send data to all connected WebSocket clients
    }
}

void loop() {
    delay(500);
    Serial.println("loop.");

    // ws_pt->cleanupClients();
    esp_err_t result;
    wifi_sta_list_t sta_list;
    result = esp_wifi_ap_get_sta_list( &sta_list);
    Serial.printf("currently connected stations to the ap: %d\n", result);
    Serial.printf("amount of connected stations to the ap: %d\n", sta_list.num);
    // Serial.printf("currently connected stations to the ap: %d\n", sta_list.sta);

    if (result == ESP_OK) {
        uint16_t aid = 0;
        result = esp_wifi_ap_get_sta_aid(sta_list.sta[0].mac, &aid);
        if (result != ESP_OK) {
            printf("Failed to get AID for the first station: %s\n", esp_err_to_name(result));
        }

    printf("The AID for the first station is: %d\n", aid);
    }


    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 100) { // Send data every 0.1 seconds
        notifyClients();
        lastSendTime = millis();
    }
}

int main() {
    setup();
    while (true) {
        loop();
    }
}
// debug backtrace with xtensa-tool
// C:\Users\maria\Dokumente\codes\C++\esp32-platformio-ESPAsyncServer\.pio\build\esp32dev>
// C:\Users\maria\.platformio\packages\toolchain-xtensa-esp32\bin\xtensa-esp32-elf-addr2line.exe -e firmware.elf 0x4014ecc8:0x3ffb21e0 0x400d9dd5:0x3ffb2200 0x400d2d8a:0x3ffb2220 0x400de102:0x3ffb2290