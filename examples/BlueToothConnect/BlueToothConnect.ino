#include "lionbitsupport.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-ba0987654321"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
String lastMessage = "";

// ---------- BLE SERVER CALLBACK ----------
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

// ---------- RECEIVE DATA CALLBACK ----------
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

void setup() {

  Serial.begin(115200);
  lionStart();

  TextDisplay("STARTING");
  delay(1000);

  BLEDevice::init("LionBit_BLE");

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

  TextDisplay("WAITING");
  Serial.println("BLE Ready");
}

void loop() {
}
