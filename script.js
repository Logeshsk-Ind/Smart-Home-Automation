
const DEVICE_NAME = "ESP32_SMART_LIGHT";

const SERVICE_UUID =
  "6e400001-b5a3-f393-e0a9-e50e24dcca9e";

const CHARACTERISTIC_UUID =
  "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

let bluetoothDevice = null;
let commandCharacteristic = null;
let isConnected = false;
let isSending = false;

const connectBtn = document.getElementById("connectBtn");
const onBtn = document.getElementById("onBtn");
const offBtn = document.getElementById("offBtn");

const connectionText = document.getElementById("connectionText");
const statusDot = document.getElementById("statusDot");
const deviceStatus = document.getElementById("deviceStatus");
const deviceIndicator = document.getElementById("deviceIndicator");
const message = document.getElementById("message");
const lightVisual = document.getElementById("lightVisual");
const lightState = document.getElementById("lightState");

function showMessage(text, type = "") {
  message.textContent = text;
  message.className = "message " + type;
}

function updateConnectionStatus(connected) {
  isConnected = connected;

  connectionText.textContent =
    connected ? "Connected" : "Disconnected";

  statusDot.classList.toggle("connected", connected);
  deviceIndicator.classList.toggle("active", connected);

  deviceStatus.textContent =
    connected ? "ESP32 is connected" : "Waiting for connection";

  onBtn.disabled = !connected;
  offBtn.disabled = !connected;

  connectBtn.disabled = false;
  connectBtn.querySelector("span").textContent =
    connected ? "Disconnect" : "Connect ESP32";
}

async function connectESP32() {
  if (!navigator.bluetooth) {
    showMessage(
      "Web Bluetooth is unavailable. Open this HTTPS website in supported Chrome on Android.",
      "error"
    );
    return;
  }

  if (isConnected && bluetoothDevice?.gatt?.connected) {
    bluetoothDevice.gatt.disconnect();
    return;
  }

  try {
    connectBtn.disabled = true;
    connectBtn.querySelector("span").textContent = "Connecting...";

    showMessage("Searching for your ESP32...");

    bluetoothDevice = await navigator.bluetooth.requestDevice({
      filters: [{ name: DEVICE_NAME }],
      optionalServices: [SERVICE_UUID]
    });

    bluetoothDevice.addEventListener(
      "gattserverdisconnected",
      handleDisconnect
    );

    showMessage("Establishing Bluetooth connection...");

    const server = await bluetoothDevice.gatt.connect();

    const service = await server.getPrimaryService(SERVICE_UUID);

    commandCharacteristic =
      await service.getCharacteristic(CHARACTERISTIC_UUID);

    updateConnectionStatus(true);

    showMessage("Your SmartLight ESP32 is connected!", "success");

  } catch (error) {
    console.error("BLE connection error:", error);

    commandCharacteristic = null;
    updateConnectionStatus(false);

    if (error.name === "NotFoundError") {
      showMessage("No ESP32 selected. Please try again.", "error");
    } else if (error.name === "NotAllowedError") {
      showMessage("Bluetooth permission was cancelled or denied.", "error");
    } else {
      showMessage("Connection failed: " + error.message, "error");
    }
  } finally {
    connectBtn.disabled = false;
    if (!isConnected) {
      connectBtn.querySelector("span").textContent = "Connect ESP32";
    }
  }
}

function handleDisconnect() {
  commandCharacteristic = null;
  updateConnectionStatus(false);

  showMessage("ESP32 disconnected. Connect again to continue.", "error");
}

async function sendCommand(command) {
  if (
    !isConnected ||
    !commandCharacteristic ||
    !bluetoothDevice?.gatt?.connected
  ) {
    showMessage("Please connect your ESP32 first.", "error");
    return;
  }

  if (isSending) return;

  try {
    isSending = true;
    onBtn.disabled = true;
    offBtn.disabled = true;

    const data = new TextEncoder().encode(command);

    await commandCharacteristic.writeValueWithResponse(data);

    if (command === "ON") {
      lightVisual.classList.add("active");
      lightState.textContent = "LIGHT ON";
      showMessage("ON command sent successfully!", "success");
    } else {
      lightVisual.classList.remove("active");
      lightState.textContent = "LIGHT OFF";
      showMessage("OFF command sent successfully!", "success");
    }

    console.log("Command sent:", command);

  } catch (error) {
    console.error("Command error:", error);
    showMessage("Unable to send command: " + error.message, "error");

  } finally {
    isSending = false;

    const connected =
      isConnected && bluetoothDevice?.gatt?.connected;

    onBtn.disabled = !connected;
    offBtn.disabled = !connected;
  }
}

connectBtn.addEventListener("click", connectESP32);

onBtn.addEventListener("click", () => {
  sendCommand("ON");
});

offBtn.addEventListener("click", () => {
  sendCommand("OFF");
});

if (!navigator.bluetooth) {
  showMessage(
    "Open this app in a Web Bluetooth-compatible browser using HTTPS."
  );
}

updateConnectionStatus(false);
