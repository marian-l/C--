import ThemeSwitch from "./components/ThemeSwitch";
import Dashboard from "./components/Dashboard";

function App() {
  return (
    <div class="mx-auto max-w-[800px] flex flex-col gap-4 p-4">
      <div class="flex w-full relative">
        <ThemeSwitch />
        <h1 class="text-5xl w-full text-center">IoT-Ambient-Music</h1>
      </div>
      {/* <SensorDisplay data={data()} />
      <AudioController /> */}
      <Dashboard />
    </div>
  );
}

export default App;
