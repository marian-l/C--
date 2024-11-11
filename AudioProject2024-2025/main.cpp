#include <iostream>
#include "maximilian.h"
#include "portaudio.h"

maxiOsc myOsc;
float temperature = 20.0; // Example temperature value; replace with actual data.

int audioCallback(const void* /*inputBuffer*/, void* outputBuffer, unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo* /*timeInfo*/, PaStreamCallbackFlags /*statusFlags*/, void* /*userData*/) {
    float* out = static_cast<float*>(outputBuffer);

    // Map temperature to frequency (e.g., 20°C to 440 Hz, 30°C to 880 Hz)
    float frequency = 440.0 + (temperature - 20.0) * 20.0;

    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        float sample = myOsc.sinewave(frequency);
        *out++ = sample;
        *out++ = sample;
    }

    return paContinue;
}

int main() {
    Pa_Initialize();
    PaStream* stream;

    Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, audioCallback, nullptr);
    Pa_StartStream(stream);

    std::cout << "Playing sound based on temperature, press Enter to stop...\n";
    std::cin.get();

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}
