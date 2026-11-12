#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ESP32Servo.h>

// ---------------- BLE CONFIGURATION ----------------

#define DEVICE_NAME "ESP32_SMART_LIGHT"

#define SERVICE_UUID \
  "6e400001-b5a3-f393-e0a9-e50e24dcca9e"

#define CHARACTERISTIC_UUID \
  "6e400002-b5a3-f393-e0a9-e50e24dcca9e"

// ---------------- HARDWARE CONFIGURATION ----------------

// GPIO numbers, not board D-labels
#define SERVO_PIN 18

// GPIO2 is a common onboard LED pin on many ESP32 DevKit boards.
// Confirm your particular board's LED pin and polarity.
#define STATUS_LED_PIN 2
#define LED_ACTIVE_LOW false

#define ON_ANGLE 0
#define OFF_ANGLE 90

// ---------------- GLOBAL VARIABLES ----------------

Servo smartServo;

BLEServer* bleServer = nullptr;
BLECharacteristic* commandCharacteristic = nullptr;

volatile bool deviceConnected = false;
volatile bool advertisingRestartNeeded = false;
volatile uint8_t pendingCommand = 0;

// ---------------- LED FUNCTION ----------------

void setStatusLED(bool connected) {
  if (LED_ACTIVE_LOW) {
    digitalWrite(STATUS_LED_PIN, connected ? LOW : HIGH);
  } else {
    digitalWrite(STATUS_LED_PIN, connected ? HIGH : LOW);
  }
}

// ---------------- BLE SERVER CALLBACKS ----------------

class ServerCallbacks : public BLEServerCallbacks {

  void onConnect(BLEServer* server) override {
    deviceConnected = true;
    Serial.println("Phone connected!");
  }

  void onDisconnect(BLEServer* server) override {
    deviceConnected = false;
    advertisingRestartNeeded = true;
    Serial.println("Phone disconnected!");
  }
};

// ---------------- BLE CHARACTERISTIC CALLBACKS ----------------

class CharacteristicCallbacks :
  public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic* characteristic) override {

    std::string receivedValue = characteristic->getValue();

    if (receivedValue == "ON") {
      pendingCommand = 1;
      Serial.println("Received command: ON");
    }

    else if (receivedValue == "OFF") {
      pendingCommand = 2;
      Serial.println("Received command: OFF");
    }

    else {
      Serial.print("Unknown command: ");
      Serial.println(receivedValue.c_str());
    }
  }
};

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("       SMARTLIGHT ESP32");
  Serial.println("================================");

  pinMode(STATUS_LED_PIN, OUTPUT);
  setStatusLED(false);

  // Attach servo
  smartServo.setPeriodHertz(50);
  smartServo.attach(SERVO_PIN, 500, 2400);

  smartServo.write(OFF_ANGLE);

  Serial.println("Servo initialized.");

  // Initialize Bluetooth
  BLEDevice::init(DEVICE_NAME);

  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new ServerCallbacks());

  BLEService* smartService =
    bleServer->createService(SERVICE_UUID);

  commandCharacteristic =
    smartService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_WRITE_NR
    );

  commandCharacteristic->setCallbacks(
    new CharacteristicCallbacks()
  );

  smartService->start();

  BLEAdvertising* advertising =
    BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println("Bluetooth initialized.");
  Serial.print("Device name: ");
  Serial.println(DEVICE_NAME);
  Serial.println("Waiting for phone connection...");
}

// ---------------- MAIN LOOP ----------------

void loop() {

  setStatusLED(deviceConnected);

  // Restart advertising after phone disconnects
  if (advertisingRestartNeeded) {
    advertisingRestartNeeded = false;

    delay(300);

    BLEDevice::startAdvertising();

    Serial.println("Advertising restarted.");
  }

  // Process received commands
  uint8_t command = pendingCommand;
  pendingCommand = 0;

  if (command == 1) {

    smartServo.write(ON_ANGLE);

    Serial.println("SmartLight ON position.");
  }

  else if (command == 2) {

    smartServo.write(OFF_ANGLE);

    Serial.println("SmartLight OFF position.");
  }

  delay(20);
}
