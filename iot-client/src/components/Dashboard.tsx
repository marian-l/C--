import { Component, createSignal, onMount, onCleanup } from "solid-js";
import { startMockDataUpdates, type SensorData } from "../data/mockData";
import SensorDisplay from "./SensorDisplay";
import AudioController from "./AudioController";

const Dashboard: Component = () => {
  const [sensorData, setSensorData] = createSignal<SensorData | null>(null);

  let ws: WebSocket;

  onMount(() => {
    const mode = import.meta.env.VITE_RUN_MODE;
    console.info(mode)

    if(mode==="prod") {
        const url = import.meta.env.VITE_WS_URL ?? 'ws://192.168.4.1/ws';
        ws = new WebSocket(url);

        ws.addEventListener("open", () => {
          console.log("WebSocket connected");
        });
    
        ws.addEventListener("message", (event) => {
          try {
            const data = JSON.parse(event.data) as SensorData;
            data.timestamp = Date.now();
            setSensorData(data);
          } catch (error) {
            console.error("Fehler beim Parsen der WebSocket-Nachricht:", error);
          }
        });
    
        ws.addEventListener("error", (error) => {
          console.error("WebSocket-Fehler:", error);
        });
    
        ws.addEventListener("close", () => {
          console.log("WebSocket closed");
        });
    }   else {
        startMockDataUpdates((newData) => {
            setSensorData(newData);
        });
    } 
  });




  onCleanup(() => {
    if (ws) {
        ws.close();
    }
  });

  return (
    <div class="grid grid-cols-1 md:grid-cols-2 gap-4 p-4">
      <SensorDisplay data={sensorData()} />
      <AudioController data={sensorData()} />
    </div>
  );
};

export default Dashboard;
