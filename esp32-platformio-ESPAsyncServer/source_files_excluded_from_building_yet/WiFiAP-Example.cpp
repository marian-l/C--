/*
  WiFiAccessPoint creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "FS.h"
#include "Wire.h"
#include "SPI.h"

#define LED_BUILTIN 2 // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "Ambient-Disco-Access-Point";
const char *password = "iot_ws24";

WiFiServer server(80);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // You can remove the password parameter if you want the AP to be open.
    WiFi.softAP(ssid, password);
    Serial.printf("AP IP address: %s\n", WiFi.softAPIP().toString().c_str());

    server.begin();
    Serial.println("Server started");
}

void loop()
{
    WiFiClient client = server.available(); // listen for incoming clients

    if (client)
    {                                  // if you get a client,
        Serial.println("New Client."); // print a message out the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        while (client.connected())
        { // loop while the client's connected
            if (client.available())
            {                         // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                if (c == '\n')
                { // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        client.print(R"rawliteral(
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
                        break;
                    }
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("Client Disconnected.");
    }
}//
// Created by maria on 02.12.2024.
//
