import { Component, createSignal, onCleanup } from "solid-js";
import type { SensorData } from "../data/mockData";
import Play from "../icons/Play";
import Stop from "../icons/Stop";

const SCALES: Record<string, number[]> = {
  "C Pentatonic": [261.63, 293.66, 329.63, 392.0, 440.0],
  "C Major": [261.63, 293.66, 329.63, 349.23, 392.0, 440.0, 493.88],
  "A Minor Pentatonic": [220.0, 246.94, 277.18, 329.63, 369.99],
  "G Pentatonic": [196.0, 220.0, 246.94, 293.66, 329.63],
  "F Major": [174.61, 196.0, 220.0, 233.08, 261.63, 293.66, 349.23],
};

const CHORD_VOICINGS = [
  [0, 2, 3, 4],
  [1, 3, 4, 0],
  [0, 1, 2, 3],
];

const BEAT_VARIATIONS: Record<string, string[]> = {
  "Only Kick": ["K", "K", "K", "K"],
  "Kick Snare": ["K", "S", "K", "S"],
  "Kick HiHat": ["K", "H", "K", "H"],
  "Kick Snare Kick HiHat": ["K", "S", "K", "H"],
  "Kick HiHat Kick Snare": ["K", "H", "K", "S"],
};

const createBitcrusherCurve = (bits = 4) => {
  const samples = 256;
  const curve = new Float32Array(samples);
  for (let i = 0; i < samples; i++) {
    const x = (i / (samples - 1)) * 2 - 1;
    const steps = Math.pow(2, bits);
    const quantized = Math.round((x + 1) * (steps / 2)) / (steps / 1) - 1;
    curve[i] = quantized;
  }
  return curve;
};

const quantizeFrequency = (freq: number) => {
  const step = 25;
  return Math.round(freq / step) * step;
};

const SOUND_PRESETS: Record<
  string,
  { melodyOsc: OscillatorType; chordOscTypes: OscillatorType[] }
> = {
  Clean: {
    melodyOsc: "sine",
    chordOscTypes: ["sine", "triangle", "sine", "triangle"],
  },
  "8-bit": {
    melodyOsc: "square",
    chordOscTypes: ["square", "square", "square", "square"],
  },
  Warm: {
    melodyOsc: "triangle",
    chordOscTypes: ["triangle", "sine", "triangle", "sawtooth"],
  },
  Harsh: {
    melodyOsc: "sawtooth",
    chordOscTypes: ["sawtooth", "square", "sawtooth", "triangle"],
  },
};

const mapTempToRootFrequency = (temp: number, scale: number[]): number => {
  const t = Math.max(0, Math.min(25, temp));
  const ratio = t / 25;
  if (scale.length < 2) return scale[0];
  return scale[0] + (scale[1] - scale[0]) * ratio;
};

interface AudioControllerProps {
  data: SensorData | null;
}

const AudioController: Component<AudioControllerProps> = (props) => {
  const [audioContext, setAudioContext] = createSignal<AudioContext | null>(
    null,
  );

  const [mainGain, setMainGain] = createSignal<GainNode | null>(null);
  const [melodyChordGain, setMelodyChordGain] = createSignal<GainNode | null>(
    null,
  );
  const [beatGain, setBeatGain] = createSignal<GainNode | null>(null);

  const [melodyIntervalTime, setMelodyIntervalTime] = createSignal(1000);
  const [chordIntervalTime, setChordIntervalTime] = createSignal(2000);
  const [bpm, setBpm] = createSignal(120);
  const [selectedBeatVariation, setSelectedBeatVariation] =
    createSignal("Kick HiHat");

  const [selectedScale, setSelectedScale] = createSignal("C Pentatonic");
  const [selectedPreset, setSelectedPreset] = createSignal("Clean");

  let melodyInterval: number | undefined;
  let beatInterval: number | undefined;
  let chordInterval: number | undefined;
  let beatStep = 0;

  const initAudio = async () => {
    if (!audioContext()) {
      const ctx = new AudioContext();
      if (ctx.state === "suspended") {
        await ctx.resume();
      }

      // Main Gain
      const mg = ctx.createGain();
      mg.gain.value = 1; // Volume

      // Melody/Chord Gain
      const mcg = ctx.createGain();
      mcg.gain.value = 0.4; // Volume

      // Beat Gain
      const bg = ctx.createGain();
      bg.gain.value = 0.5; // Volume

      if (selectedPreset() === "8-bit") {
        // Bit crusher
        const bitcrusher = ctx.createWaveShaper();
        bitcrusher.curve = createBitcrusherCurve(4);
        bitcrusher.oversample = "none";

        mcg.connect(bitcrusher).connect(mg);
        bg.connect(mg);
        mg.connect(ctx.destination);
      } else {
        // No bit crusher
        mcg.connect(mg);
        bg.connect(mg);
        mg.connect(ctx.destination);
      }

      setMainGain(mg);
      setMelodyChordGain(mcg);
      setBeatGain(bg);
      setAudioContext(ctx);

      startIntervals(ctx, mcg, bg);
    } else {
      const ctx = audioContext()!;
      if (ctx.state === "suspended") {
        await ctx.resume();
      }
      if (mainGain() && melodyChordGain() && beatGain()) {
        stopIntervals();
        startIntervals(ctx, melodyChordGain()!, beatGain()!);
      }
    }
  };

  const startIntervals = (ctx: AudioContext, mcg: GainNode, bg: GainNode) => {
    stopIntervals();

    melodyInterval = setInterval(() => {
      if (!props.data) return;
      const scale = SCALES[selectedScale()] || SCALES["C Pentatonic"];
      const pressure = (props.data.pressure ?? 1015) - 980;
      const amplifiedPressure = pressure * 1.5;
      const noteIdx = Math.floor((amplifiedPressure / 70) * scale.length) % scale.length;
      
      triggerMelodyNote(
        ctx,
        mcg,
        scale[noteIdx],
        SOUND_PRESETS[selectedPreset()].melodyOsc,
      );
    }, melodyIntervalTime());

    chordInterval = setInterval(() => {
      if (!props.data) return;
      const scale = SCALES[selectedScale()] || SCALES["C Pentatonic"];
      triggerChord(
        ctx,
        mcg,
        props.data,
        scale,
        SOUND_PRESETS[selectedPreset()].chordOscTypes,
      );
    }, chordIntervalTime());

    const pattern =
      BEAT_VARIATIONS[selectedBeatVariation()] || BEAT_VARIATIONS["Kick HiHat"];
    beatStep = 0;
    beatInterval = setInterval(() => {
      if (beatGain()) {
        triggerBeat(ctx, bg, pattern, beatStep);
        beatStep = (beatStep + 1) % pattern.length;
      }
    }, 60_000 / bpm());
  };

  const stopIntervals = () => {
    if (melodyInterval) clearInterval(melodyInterval);
    if (beatInterval) clearInterval(beatInterval);
    if (chordInterval) clearInterval(chordInterval);
  };

  const stopAudio = async () => {
    stopIntervals();
    audioContext()?.close();
    setAudioContext(null);
  };

  onCleanup(() => {
    stopIntervals();
    audioContext()?.close();
  });

  const triggerMelodyNote = (
    ctx: AudioContext,
    mcg: GainNode,
    freq: number,
    oscType: OscillatorType,
  ) => {
    if (selectedPreset() === "8-bit") {
      freq = quantizeFrequency(freq);
    }

    const osc = ctx.createOscillator();
    osc.type = oscType;
    osc.frequency.value = freq;

    const g = ctx.createGain();
    g.gain.value = 0.0;

    osc.connect(g).connect(mcg);
    osc.start(ctx.currentTime);

    const now = ctx.currentTime;

    g.gain.linearRampToValueAtTime(0.3, now + 0.05);

    if (selectedPreset() === "8-bit") {
      //Short sounds for 8-bit with little release
      g.gain.linearRampToValueAtTime(0.0, now + 0.25);
      osc.stop(now + 0.3);
    } else {
      g.gain.linearRampToValueAtTime(0.0, now + 1.0);
      osc.stop(now + 1.5);
    }
  };

  const triggerChord = (
    ctx: AudioContext,
    mcg: GainNode,
    data: SensorData,
    scale: number[],
    chordOscTypes: OscillatorType[],
  ) => {
    const { brightness, humidity, temperature } = data;

    const rootFreq = mapTempToRootFrequency(temperature, scale);
    const minH = 40;
    const maxH = 80;
    const hRatio =
      (Math.min(Math.max(humidity, minH), maxH) - minH) / (maxH - minH);
    const chordIndex =
      Math.floor(hRatio * CHORD_VOICINGS.length) % CHORD_VOICINGS.length;
    const voicing = CHORD_VOICINGS[chordIndex];

    const notes = voicing.map((i, idx) => {
      let f = idx === 0 ? rootFreq : scale[i % scale.length];
      if (selectedPreset() === "8-bit") {
        f = quantizeFrequency(f);
      }
      return f;
    });

    let chordGain = 0.1 + ((brightness || 0) / 1000) * 0.5;

    const now = ctx.currentTime;

    notes.forEach((freq, i) => {
      const osc = ctx.createOscillator();
      osc.type = chordOscTypes[i % chordOscTypes.length];
      osc.frequency.value = freq;

      const g = ctx.createGain();
      g.gain.value = 0.0;

      osc.connect(g).connect(mcg);

      if (selectedPreset() === "8-bit") {
        //Short sounds for 8-bit with little release
        g.gain.linearRampToValueAtTime(chordGain, now + 0.05);
        g.gain.linearRampToValueAtTime(0.0, now + 0.15);
        osc.start(now);
        osc.stop(now + 0.2);
      } else {
        g.gain.linearRampToValueAtTime(chordGain, now + 0.1);
        g.gain.linearRampToValueAtTime(0.0, now + 1.5);
        osc.start(now);
        osc.stop(now + 2.0);
      }
    });
  };

  const triggerBeat = (
    ctx: AudioContext,
    bg: GainNode,
    pattern: string[],
    step: number,
  ) => {
    const hit = pattern[step];
    if (hit === "K") {
      triggerKick(ctx, bg);
    } else if (hit === "H") {
      triggerHiHat(ctx, bg);
    } else if (hit === "S") {
      triggerSnare(ctx, bg);
    }
  };

  const triggerKick = (ctx: AudioContext, bg: GainNode) => {
    const osc = ctx.createOscillator();
    osc.type = "sine";
    osc.frequency.setValueAtTime(100, ctx.currentTime);

    const g = ctx.createGain();
    g.gain.setValueAtTime(0, ctx.currentTime);
    g.gain.linearRampToValueAtTime(1.0, ctx.currentTime + 0.01);
    g.gain.linearRampToValueAtTime(0, ctx.currentTime + 0.15);

    osc.connect(g).connect(bg);
    osc.start();
    osc.stop(ctx.currentTime + 0.2);
  };

  const triggerHiHat = (ctx: AudioContext, bg: GainNode) => {
    const bufferSize = 2 * ctx.sampleRate;
    const buffer = ctx.createBuffer(1, bufferSize, ctx.sampleRate);
    const data = buffer.getChannelData(0);
    for (let i = 0; i < bufferSize; i++) {
      data[i] = Math.random() * 2 - 1;
    }

    const source = ctx.createBufferSource();
    source.buffer = buffer;

    const bandpass = ctx.createBiquadFilter();
    bandpass.type = "bandpass";
    bandpass.frequency.value = 5000;

    const g = ctx.createGain();
    g.gain.setValueAtTime(0, ctx.currentTime);
    g.gain.linearRampToValueAtTime(1.0, ctx.currentTime + 0.005);
    g.gain.linearRampToValueAtTime(0, ctx.currentTime + 0.05);

    source.connect(bandpass).connect(g).connect(bg);
    source.start();
    source.stop(ctx.currentTime + 0.1);
  };

  const triggerSnare = (ctx: AudioContext, bg: GainNode) => {
    const bufferSize = 2 * ctx.sampleRate;
    const buffer = ctx.createBuffer(1, bufferSize, ctx.sampleRate);
    const data = buffer.getChannelData(0);
    for (let i = 0; i < bufferSize; i++) {
      data[i] = Math.random() * 2 - 1;
    }

    const source = ctx.createBufferSource();
    source.buffer = buffer;

    const bandpass = ctx.createBiquadFilter();
    bandpass.type = "bandpass";
    bandpass.frequency.value = 2000;

    const g = ctx.createGain();
    g.gain.setValueAtTime(0, ctx.currentTime);
    g.gain.linearRampToValueAtTime(1.0, ctx.currentTime + 0.01);
    g.gain.linearRampToValueAtTime(0, ctx.currentTime + 0.2);

    source.connect(bandpass).connect(g).connect(bg);
    source.start();
    source.stop(ctx.currentTime + 0.3);
  };

  return (
    <div class="flex flex-col gap-4 bg-neutral-300 dark:bg-neutral-700 p-4 rounded">
      <h2 class="text-3xl w-full text-center pb-2 border-b-2 border-neutral-100">
        Audio Controller
      </h2>
      <div>
        <label class="flex gap-2 w-full items-center">
          <span class="w-1/4">Melody Interval (ms):</span>
          <input
            type="number"
            class="rounded p-2 ring-1 ring-neutral-800 w-full bg-neutral-100 dark:bg-neutral-500 dark:ring-neutral-100 disabled:opacity-50"
            value={melodyIntervalTime()}
            disabled={audioContext() ? true : false}
            onInput={(e) => {
              let val = parseInt((e.target as HTMLInputElement).value);
              setMelodyIntervalTime(val);
            }}
          />
        </label>
      </div>

      <div>
        <label class="flex gap-2 w-full items-center">
          <span class="w-1/4">Chord Interval (ms):</span>
          <input
            type="number"
            class="rounded p-2 ring-1 ring-neutral-800 w-full bg-neutral-100 dark:bg-neutral-500 dark:ring-neutral-100 disabled:opacity-50"
            value={chordIntervalTime()}
            disabled={audioContext() ? true : false}
            onInput={(e) => {
              let val = parseInt((e.target as HTMLInputElement).value);
              setChordIntervalTime(val);
            }}
          />
        </label>
      </div>

      <div>
        <label class="flex gap-2 w-full items-center">
          <span class="w-1/4">BPM:</span>
          <input
            type="number"
            class="rounded p-2 ring-1 ring-neutral-800 w-full bg-neutral-100 dark:bg-neutral-500 dark:ring-neutral-100 disabled:opacity-50"
            value={bpm()}
            disabled={audioContext() ? true : false}
            onInput={(e) => {
              let val = parseInt((e.target as HTMLInputElement).value);
              setBpm(val);
            }}
          />
        </label>
      </div>

      <div>
        <label class="flex gap-2 w-full items-center">
          <span class="w-1/4">Beat Variation:</span>
          <select
            class="rounded p-2 ring-1 ring-neutral-800 appearance-none w-full cursor-pointer bg-neutral-100 dark:bg-neutral-500 dark:ring-neutral-100 disabled:opacity-50"
            disabled={audioContext() ? true : false}
            value={selectedBeatVariation()}
            onInput={(e) =>
              setSelectedBeatVariation((e.target as HTMLSelectElement).value)
            }
          >
            {Object.keys(BEAT_VARIATIONS).map((key) => (
              <option value={key}>{key}</option>
            ))}
          </select>
        </label>
      </div>

      <div>
        <label class="flex gap-2 w-full items-center">
          <span class="w-1/4">Key:</span>
          <select
            class="rounded p-2 ring-1 ring-neutral-800 appearance-none w-full cursor-pointer bg-neutral-100 dark:bg-neutral-500 dark:ring-neutral-100 disabled:opacity-50"
            disabled={audioContext() ? true : false}
            value={selectedScale()}
            onInput={(e) =>
              setSelectedScale((e.target as HTMLSelectElement).value)
            }
          >
            {Object.keys(SCALES).map((key) => (
              <option value={key}>{key}</option>
            ))}
          </select>
        </label>
      </div>

      <div>
        <label class="flex gap-2 w-full items-center">
          <span class="w-1/4">Sound-Design:</span>
          <select
            class="rounded p-2 ring-1 ring-neutral-800 appearance-none w-full cursor-pointer bg-neutral-100 dark:bg-neutral-500 dark:ring-neutral-100 disabled:opacity-50"
            disabled={audioContext() ? true : false}
            value={selectedPreset()}
            onInput={(e) =>
              setSelectedPreset((e.target as HTMLSelectElement).value)
            }
          >
            {Object.keys(SOUND_PRESETS).map((key) => (
              <option value={key}>{key}</option>
            ))}
          </select>
        </label>
      </div>
      <div class="flex justify-center">
        {!audioContext() && (
          <button
            class="rounded-full p-4 cursor-pointer bg-green-700 font-bold text-neutral-100 flex items-center justify-center hover:bg-green-600 transition-all duration-300 ease-in-out"
            onClick={initAudio}
          >
            <Play />
          </button>
        )}
        {audioContext() && (
          <button
            class="rounded-full p-4 cursor-pointer bg-red-700 text-neutral-100 font-bold flex items-center justify-center hover:bg-red-600 transition-all duration-300 ease-in-out"
            onClick={stopAudio}
          >
            <Stop />
          </button>
        )}
      </div>
    </div>
  );
};

export default AudioController;
