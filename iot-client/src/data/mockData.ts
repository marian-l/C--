export interface SensorData {
  brightness: number;
  noise: number;
  humidity: number;
  pressure: number;
  temperature: number;
  timestamp: number;
}

let currentData: SensorData = {
  brightness: 200,
  noise: 50,
  humidity: 60,
  pressure: 1015,
  temperature: 15,
  timestamp: Date.now(),
};

function gentleRandomChange(
  value: number,
  amount: number,
  min: number,
  max: number,
): number {
  const newValue = value + (Math.random() - 0.5) * amount * 2;
  return Math.max(min, Math.min(max, newValue));
}

function simulateDataStep() {
  currentData.timestamp = Date.now();

  currentData.temperature = gentleRandomChange(
    currentData.temperature,
    5,
    0,
    25,
  );

  currentData.humidity = gentleRandomChange(currentData.humidity, 10, 40, 80);

  currentData.pressure = gentleRandomChange(currentData.pressure, 5, 980, 1050);

  currentData.brightness = gentleRandomChange(
    currentData.brightness,
    20,
    0,
    1000,
  );

  currentData.noise = gentleRandomChange(currentData.noise, 2, 30, 70);
}

export function startMockDataUpdates(callback: (data: SensorData) => void) {
  setInterval(() => {
    simulateDataStep();
    callback({ ...currentData });
  }, 100);
}
