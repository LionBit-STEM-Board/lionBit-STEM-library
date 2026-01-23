#include "lionbitsupport.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// ================= FUNCTION DECLARATIONS =================
void moveForward(int speedVal);
void moveReverse(int speedVal);
void turnLeft(int speedVal);
void turnRight(int speedVal);
void stopMotors();

// ================= BLE UUIDs =================
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-ba0987654321"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
String lastMessage = "";

// ================= MOTOR PINS (L298N) =================
#define ENA D5
#define IN1 D6
#define IN2 D7

#define ENB D8
#define IN3 D9
#define IN4 D10

int carSpeed = 180;   // Change speed here (0–255)

// ================= BLE SERVER CALLBACK =================
class MyServerCallbacks : public BLEServerCallbacks {

  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    TextDisplay("CONNECTED");
    Serial.println("BLE Connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    TextDisplay("WAITING");
    Serial.println("BLE Disconnected");
    BLEDevice::startAdvertising();
  }
};

// ================= RECEIVE DATA CALLBACK =================
class MyCallbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) {

    String cmd = pCharacteristic->getValue();
    cmd.trim();
    cmd.toUpperCase();

    if (cmd.length() == 0) return;
    if (cmd == lastMessage) return;
    lastMessage = cmd;

    Serial.print("Received: ");
    Serial.println(cmd);
class MyCallbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) {

    String receivedText = pCharacteristic->getValue();
    receivedText.trim();

    if (receivedText.length() == 0) return;

    if (receivedText == lastMessage) return;
    lastMessage = receivedText;

    Serial.print("Received: ");
    Serial.println(receivedText);

    if (receivedText.equalsIgnoreCase("clear")) {
      ClsDis();
    }
    else {
      TextDisplay(receivedText);
    }
  }

  
};

    

// ================= SETUP =================
void setup() {

  Serial.begin(115200);
  lionStart();
 TextColor(ST7735_YELLOW);
  TextSize(2);

  // ---- Motor Pins ----
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  // ---- Display ----
  TextDisplay("STARTING");
  delay(1000);

  // ---- BLE INIT ----
  BLEDevice::init("LionBit_CAR");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_READ
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  BLEDevice::getAdvertising()->start();

  TextDisplay(" --- WAITING");
  Serial.println("BLE Ready");
}

// ================= LOOP =================
void loop() {
  // BLE handled by callbacks
}

// ================= MOTOR FUNCTIONS =================

// Both motors forward
void moveForward(int speedVal) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

// Both motors backward
void moveReverse(int speedVal) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

// Smooth left turn (left slow, right fast)
void turnLeft(int speedVal) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedVal / 3);  // left motor slow
  analogWrite(ENB, speedVal);      // right motor fast
}

// Smooth right turn (right slow, left fast)
void turnRight(int speedVal) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedVal);      // left motor fast
  analogWrite(ENB, speedVal / 3);  // right motor slow
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
