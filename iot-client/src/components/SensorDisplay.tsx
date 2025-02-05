import { Component } from "solid-js";
import type { SensorData } from "../data/mockData";

interface SensorDisplayProps {
  data: SensorData | null;
}

const SensorDisplay: Component<SensorDisplayProps> = (props) => {
  return (
    <div class="flex flex-col gap-2 bg-neutral-300 dark:bg-neutral-700 p-4 rounded">
      <h2 class="text-3xl w-full text-center pb-2 border-b-2 border-neutral-100">
        Sensor Data
      </h2>
      {props.data ? (
        <table>
          <tbody>
            <tr>
              <td class="w-1/2">Brightness:</td>
              <td class="w-1/2">{props.data.brightness.toFixed(2)} lux</td>
            </tr>
            <tr>
              <td class="w-1/2">Noise:</td>
              <td class="w-1/2">{props.data.noise.toFixed(2)} dB</td>
            </tr>
            <tr>
              <td class="w-1/2">Humidity:</td>
              <td class="w-1/2">{props.data.humidity.toFixed(2)} %</td>
            </tr>
            <tr>
              <td class="w-1/2">Pressure:</td>
              <td class="w-1/2">{props.data.pressure.toFixed(2)} hPa</td>
            </tr>
            <tr>
              <td class="w-1/2">Temperature:</td>
              <td class="w-1/2">{props.data.temperature.toFixed(2)} °C</td>
            </tr>
            <tr>
              <td class="w-1/2">Last Update:</td>
              <td class="w-1/2">
                {new Date(props.data.timestamp).toLocaleTimeString()}
              </td>
            </tr>
          </tbody>
        </table>
      ) : (
        <p>No Data</p>
      )}
    </div>
  );
};

export default SensorDisplay;
