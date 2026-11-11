<<<<<<< HEAD

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <ESP32Servo.h>

// ---------------- PIN CONFIGURATION ----------------

#define SERVO_PIN 18
#define LED_PIN 23

// ---------------- SERVO CONFIGURATION ----------------

#define ON_ANGLE 0
#define OFF_ANGLE 90

Servo myServo;

// ---------------- BLE CONFIGURATION ----------------

// Must match the JavaScript application exactly

#define SERVICE_UUID \
"6e400001-b5a3-f393-e0a9-e50e24dcca9e"

#define CHARACTERISTIC_UUID \
"6e400002-b5a3-f393-e0a9-e50e24dcca9e"

BLECharacteristic *commandCharacteristic;

// 0 = No command
// 1 = ON
// 2 = OFF

volatile int pendingCommand = 0;

// ---------------- BLE CALLBACK ----------------

class CommandCallbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) override {

    std::string receivedValue =
      pCharacteristic->getValue();

    String command = String(receivedValue.c_str());

    command.trim();
    command.toUpperCase();

    if (command == "ON") {

      pendingCommand = 1;

    }

    else if (command == "OFF") {

      pendingCommand = 2;

    }

  }

};

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(115200);

  // Configure LED

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Configure servo

  myServo.setPeriodHertz(50);

  myServo.attach(SERVO_PIN, 500, 2400);

  myServo.write(OFF_ANGLE);

  // Initialize BLE

  BLEDevice::init("ESP32_SMART_LIGHT");

  // Create BLE server

  BLEServer *server = BLEDevice::createServer();

  // Create BLE service

  BLEService *service =
    server->createService(SERVICE_UUID);

  // Create writable characteristic

  commandCharacteristic =
    service->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE
    );

  // Register command callback

  commandCharacteristic->setCallbacks(
    new CommandCallbacks()
  );

  // Start service

  service->start();

  // Start advertising

  BLEAdvertising *advertising =
    BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);

  advertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println("---------------------------");
  Serial.println(" SMARTLIGHT SYSTEM READY ");
  Serial.println("---------------------------");

  Serial.println("Bluetooth: ESP32_SMART_LIGHT");
  Serial.println("Waiting for phone connection...");

}

// ---------------- MAIN LOOP ----------------

void loop() {

  int command = pendingCommand;

  if (command != 0) {

    // Clear command before processing

    pendingCommand = 0;

    if (command == 1) {

      // LIGHT ON

      myServo.write(ON_ANGLE);

      digitalWrite(LED_PIN, HIGH);

      Serial.println("LIGHT ON");
      Serial.println("Servo moved to ON position");

    }

    else if (command == 2) {

      // LIGHT OFF

      myServo.write(OFF_ANGLE);

      digitalWrite(LED_PIN, LOW);

      Serial.println("LIGHT OFF");
      Serial.println("Servo moved to OFF position");

    }

  }

  delay(10);

}
=======

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <ESP32Servo.h>

// ---------------- PIN CONFIGURATION ----------------

#define SERVO_PIN 18
#define LED_PIN 23

// ---------------- SERVO CONFIGURATION ----------------

#define ON_ANGLE 0
#define OFF_ANGLE 90

Servo myServo;

// ---------------- BLE CONFIGURATION ----------------

// Must match the JavaScript application exactly

#define SERVICE_UUID \
"6e400001-b5a3-f393-e0a9-e50e24dcca9e"

#define CHARACTERISTIC_UUID \
"6e400002-b5a3-f393-e0a9-e50e24dcca9e"

BLECharacteristic *commandCharacteristic;

// 0 = No command
// 1 = ON
// 2 = OFF

volatile int pendingCommand = 0;

// ---------------- BLE CALLBACK ----------------

class CommandCallbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) override {

    std::string receivedValue =
      pCharacteristic->getValue();

    String command = String(receivedValue.c_str());

    command.trim();
    command.toUpperCase();

    if (command == "ON") {

      pendingCommand = 1;

    }

    else if (command == "OFF") {

      pendingCommand = 2;

    }

  }

};

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(115200);

  // Configure LED

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Configure servo

  myServo.setPeriodHertz(50);

  myServo.attach(SERVO_PIN, 500, 2400);

  myServo.write(OFF_ANGLE);

  // Initialize BLE

  BLEDevice::init("ESP32_SMART_LIGHT");

  // Create BLE server

  BLEServer *server = BLEDevice::createServer();

  // Create BLE service

  BLEService *service =
    server->createService(SERVICE_UUID);

  // Create writable characteristic

  commandCharacteristic =
    service->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE
    );

  // Register command callback

  commandCharacteristic->setCallbacks(
    new CommandCallbacks()
  );

  // Start service

  service->start();

  // Start advertising

  BLEAdvertising *advertising =
    BLEDevice::getAdvertising();

  advertising->addServiceUUID(SERVICE_UUID);

  advertising->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println("---------------------------");
  Serial.println(" SMARTLIGHT SYSTEM READY ");
  Serial.println("---------------------------");

  Serial.println("Bluetooth: ESP32_SMART_LIGHT");
  Serial.println("Waiting for phone connection...");

}

// ---------------- MAIN LOOP ----------------

void loop() {

  int command = pendingCommand;

  if (command != 0) {

    // Clear command before processing

    pendingCommand = 0;

    if (command == 1) {

      // LIGHT ON

      myServo.write(ON_ANGLE);

      digitalWrite(LED_PIN, HIGH);

      Serial.println("LIGHT ON");
      Serial.println("Servo moved to ON position");

    }

    else if (command == 2) {

      // LIGHT OFF

      myServo.write(OFF_ANGLE);

      digitalWrite(LED_PIN, LOW);

      Serial.println("LIGHT OFF");
      Serial.println("Servo moved to OFF position");

    }

  }

  delay(10);

}
>>>>>>> 66052b3e2dc0a77bfa2090aecf9421924c3f869b
