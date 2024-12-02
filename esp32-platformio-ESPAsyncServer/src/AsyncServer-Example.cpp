#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <SPI.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char* ssid =     "ambient_disco_ap";
const char* password = "rocking_stone";

const char* rootPage = R"(
R"rawliteral(
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
)";

void handleNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void handleRoot(AsyncWebServerRequest* request) {
    request->send(200, "text/html", rootPage);
}

void setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
}

void setupWiFiAP() {
    WiFi.softAP(ssid, password);
}

void setup() {
    Serial.begin(115200);

    setupWiFiAP();
    setupWebServer();
}

void loop() {
    delay(5000);
    Serial.println("Beep beep booop.");
}

int main() {
    setup();
    while (true) {
        loop();
    }
}