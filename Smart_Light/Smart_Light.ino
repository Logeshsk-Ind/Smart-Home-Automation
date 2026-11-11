
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ESP32Servo.h>

// =====================================
// SMARTLIGHT - ESP32 BLE FIRMWARE
// =====================================

// Hardware configuration
#define SERVO_PIN 18
#define STATUS_LED_PIN 2

// Servo positions: calibrate for your switch
#define ON_ANGLE 0
#define OFF_ANGLE 90

// Change to true if your blue LED is active LOW
#define LED_ACTIVE_LOW false

// BLE configuration
#define DEVICE_NAME "ESP32_SMART_LIGHT"

#define SERVICE_UUID \
"6e400001-b5a3-f393-e0a9-e50e24dcca9e"

#define CHARACTERISTIC_UUID \
"6e400002-b5a3-f393-e0a9-e50e24dcca9e"

// =====================================
// GLOBAL OBJECTS
// =====================================

Servo myServo;

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;

volatile bool deviceConnected = false;
volatile uint8_t pendingCommand = 0;

// =====================================
// BLUE LED CONTROL
// =====================================

void setStatusLED(bool state) {

  if (LED_ACTIVE_LOW) {
    digitalWrite(
      STATUS_LED_PIN,
      state ? LOW : HIGH
    );
  }
  else {
    digitalWrite(
      STATUS_LED_PIN,
      state ? HIGH : LOW
    );
  }
}

// =====================================
// BLE SERVER CALLBACKS
// =====================================

class ServerCallbacks : public BLEServerCallbacks {

  void onConnect(BLEServer *server) override {

    deviceConnected = true;

    Serial.println("Phone connected!");
  }

  void onDisconnect(BLEServer *server) override {

    deviceConnected = false;

    pendingCommand = 0;

    Serial.println("Phone disconnected!");

    // Advertise again for the next connection
    BLEDevice::startAdvertising();
  }
};

// =====================================
// BLE COMMAND CALLBACKS
// =====================================

class CommandCallbacks :
  public BLECharacteristicCallbacks {

  void onWrite(
    BLECharacteristic *characteristic
  ) override {

    auto receivedValue =
      characteristic->getValue();

    String command = receivedValue.c_str();

    command.trim();
    command.toUpperCase();

    if (command == "ON") {

      pendingCommand = 1;

      Serial.println("ON command received");

    }

    else if (command == "OFF") {

      pendingCommand = 2;

      Serial.println("OFF command received");

    }

    else {

      Serial.println("Invalid command received");

    }
  }
};

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(115200);

  // Blue LED
  pinMode(STATUS_LED_PIN, OUTPUT);
  setStatusLED(false);

  // Servo
  myServo.setPeriodHertz(50);

  myServo.attach(
    SERVO_PIN,
    500,
    2400
  );

  myServo.write(OFF_ANGLE);

  // BLE initialization
  BLEDevice::init(DEVICE_NAME);

  // Create BLE server
  pServer = BLEDevice::createServer();

  pServer->setCallbacks(
    new ServerCallbacks()
  );

  // Create BLE service
  BLEService *pService =
    pServer->createService(SERVICE_UUID);

  // Create writable characteristic
  pCharacteristic =
    pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE
    );

  pCharacteristic->setCallbacks(
    new CommandCallbacks()
  );

  // Start service
  pService->start();

  // BLE advertising
  BLEAdvertising *pAdvertising =
    BLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println();
  Serial.println("==============================");
  Serial.println("   SMARTLIGHT SYSTEM READY");
  Serial.println("==============================");

  Serial.print("Device name: ");
  Serial.println(DEVICE_NAME);

  Serial.println("BLE advertising started");
  Serial.println("Waiting for smartphone...");
}

// =====================================
// MAIN LOOP
// =====================================

void loop() {

  // Blue LED indicates BLE connection
  setStatusLED(deviceConnected);

  // Process received commands
  uint8_t command = pendingCommand;

  if (command != 0) {

    pendingCommand = 0;

    if (command == 1) {

      // LIGHT ON
      myServo.write(ON_ANGLE);

      Serial.println("LIGHT ON");
      Serial.println("Servo: ON position");

    }

    else if (command == 2) {

      // LIGHT OFF
      myServo.write(OFF_ANGLE);

      Serial.println("LIGHT OFF");
      Serial.println("Servo: OFF position");

    }
  }

  delay(10);
}
