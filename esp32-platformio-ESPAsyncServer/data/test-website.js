
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

