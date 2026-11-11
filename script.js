<<<<<<< HEAD

const SERVICE_UUID =
  "6e400001-b5a3-f393-e0a9-e50e24dcca9e";

const CHARACTERISTIC_UUID =
  "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

let device;
let commandCharacteristic;

const connectBtn =
  document.getElementById("connectBtn");

const onBtn = document.getElementById("onBtn");
const offBtn = document.getElementById("offBtn");

async function connectESP32() {
  try {
    device = await navigator.bluetooth.requestDevice({
      filters: [{ name: "ESP32_SMART_LIGHT" }],
      optionalServices: [SERVICE_UUID]
    });

    const server = await device.gatt.connect();

    const service =
      await server.getPrimaryService(SERVICE_UUID);

    commandCharacteristic =
      await service.getCharacteristic(CHARACTERISTIC_UUID);

    document.getElementById("connectionStatus").textContent =
      "ESP32: Connected";

    connectBtn.textContent = "Connected";

  } catch (error) {
    alert("Bluetooth error: " + error.message);
  }
}

async function sendCommand(command) {
  if (!commandCharacteristic) {
    alert("Connect ESP32 first!");
    return;
  }

  try {
    const data = new TextEncoder().encode(command);

    await commandCharacteristic.writeValueWithResponse(data);

    updateUI(command);

  } catch (error) {
    alert("Command failed: " + error.message);
  }
}

function updateUI(command) {
  const isOn = command === "ON";

  document.getElementById("lightStatus").textContent =
    isOn ? "ON" : "OFF";

  document.getElementById("lightStatus").className =
    isOn ? "status on" : "status off";

  document.getElementById("message").textContent =
    isOn ? "Your light is switched ON" :
           "Your light is switched OFF";

  document.getElementById("lightIcon").style.opacity =
    isOn ? "1" : "0.4";
}

connectBtn.addEventListener("click", connectESP32);

onBtn.addEventListener("click", () => {
  sendCommand("ON");
});

offBtn.addEventListener("click", () => {
  sendCommand("OFF");
});
=======

const SERVICE_UUID =
  "6e400001-b5a3-f393-e0a9-e50e24dcca9e";

const CHARACTERISTIC_UUID =
  "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

let device;
let commandCharacteristic;

const connectBtn =
  document.getElementById("connectBtn");

const onBtn = document.getElementById("onBtn");
const offBtn = document.getElementById("offBtn");

async function connectESP32() {
  try {
    device = await navigator.bluetooth.requestDevice({
      filters: [{ name: "ESP32_SMART_LIGHT" }],
      optionalServices: [SERVICE_UUID]
    });

    const server = await device.gatt.connect();

    const service =
      await server.getPrimaryService(SERVICE_UUID);

    commandCharacteristic =
      await service.getCharacteristic(CHARACTERISTIC_UUID);

    document.getElementById("connectionStatus").textContent =
      "ESP32: Connected";

    connectBtn.textContent = "Connected";

  } catch (error) {
    alert("Bluetooth error: " + error.message);
  }
}

async function sendCommand(command) {
  if (!commandCharacteristic) {
    alert("Connect ESP32 first!");
    return;
  }

  try {
    const data = new TextEncoder().encode(command);

    await commandCharacteristic.writeValueWithResponse(data);

    updateUI(command);

  } catch (error) {
    alert("Command failed: " + error.message);
  }
}

function updateUI(command) {
  const isOn = command === "ON";

  document.getElementById("lightStatus").textContent =
    isOn ? "ON" : "OFF";

  document.getElementById("lightStatus").className =
    isOn ? "status on" : "status off";

  document.getElementById("message").textContent =
    isOn ? "Your light is switched ON" :
           "Your light is switched OFF";

  document.getElementById("lightIcon").style.opacity =
    isOn ? "1" : "0.4";
}

connectBtn.addEventListener("click", connectESP32);

onBtn.addEventListener("click", () => {
  sendCommand("ON");
});

offBtn.addEventListener("click", () => {
  sendCommand("OFF");
});
>>>>>>> 66052b3e2dc0a77bfa2090aecf9421924c3f869b
