import { Checkbox } from "@kobalte/core/checkbox";
import { Component, createSignal, onMount } from "solid-js";
import Moon from "../icons/Moon";
import Sun from "../icons/Sun";

type Props = {
  class?: string;
};

const ThemeSwitch: Component<Props> = (props) => {
  const [isDarkMode, setIsDarkMode] = createSignal(false);

  onMount(() => {
    const darkModePreference = localStorage.getItem("darkMode");
    if (
      darkModePreference === "enabled" ||
      (!darkModePreference &&
        window.matchMedia("(prefers-color-scheme: dark)").matches)
    ) {
      document.documentElement.classList.add("dark");
      setIsDarkMode(true);
    }
  });

  const toggleDarkMode = () => {
    if (isDarkMode()) {
      document.documentElement.classList.remove("dark");
      localStorage.setItem("darkMode", "disabled");
      setIsDarkMode(false);
    } else {
      document.documentElement.classList.add("dark");
      localStorage.setItem("darkMode", "enabled");
      setIsDarkMode(true);
    }
  };

  return (
    <div class="absolute top-1/2 transform -translate-y-1/2 right-4 flex items-center">
      <Checkbox
        checked={isDarkMode()}
        onChange={toggleDarkMode}
        class={`inline-flex items-center cursor-pointer ${props.class || ""}`}
      >
        <Checkbox.Input class="sr-only peer" />
        <Checkbox.Control class="relative w-11 h-6 bg-neutral-700 dark:bg-neutral-100 peer-focus:outline-none peer-focus:ring-4 peer-focus:ring-neutral-300 dark:peer-focus:ring-neutral-600 rounded-full peer peer-checked:after:translate-x-full rtl:peer-checked:after:-translate-x-full peer-checked:after:border-neutral-100 after:content-[''] after:absolute after:top-[2px] after:start-[2px] after:bg-neutral-100 dark:after:bg-neutral-800 after:border-neutral-100 dark:after:border-neutral-800 after:border after:rounded-full after:h-5 after:w-5 after:transition-all peer-checked:bg-neutral-100">
          <Checkbox.Indicator></Checkbox.Indicator>
        </Checkbox.Control>
        <Checkbox.Label class="ms-3 text-sm font-medium text-neutral-800 dark:text-neutral-100">
          {isDarkMode() ? <Moon /> : <Sun />}
        </Checkbox.Label>
      </Checkbox>
    </div>
  );
};

export default ThemeSwitch;
