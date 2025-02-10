# Ambient Disco - Frontend (Audio-Controller)
## Prerequisites
- pnpm
- NodeJS >= 20

## Install dependencies

```bash
$ pnpm install
```


## Environment-Variables

Copy the content of `.env.sample` and paste it into a new file called `.env`. 

## Simulating Data

If you want to use a build for the ESP32 (receiving real sensor data), then `VITE_RUN_MODE=prod`. If you want to simulate data in the frontend, then `VITE_RUN_MODE=dev`.

## Available Scripts

In the project directory, you can run:

### `pnpm dev`

Runs the app in the development mode.<br>
Open [http://localhost:5173](http://localhost:5173) to view it in the browser.

### `pnpm dev`

Runs the app in the preview mode (Simulating a statically served frontend).<br>
Open [http://localhost:4173](http://localhost:4173) to view it in the browser.

### `pnpm build`

Builds the app for production to the `dist` folder.<br>
It correctly bundles Solid in production mode and optimizes the build for the best performance.

The build is minified and the filenames include the hashes.<br>
Your app is ready to be deployed!

## Deployment

The content of the dist directory needs to be copied into the directory `/data` in the ESP32 project to be served statically.
