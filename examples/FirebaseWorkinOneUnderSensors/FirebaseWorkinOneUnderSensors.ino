#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "lionbitsupport.h"  // LionBit functions: lionStart(), ClsDis(), TextDisplay()

// ===== WiFi Credentials =====
#define WIFI_SSID "LionBit"
#define WIFI_PASSWORD "1234567890"

// ===== Firebase Settings =====
#define API_KEY "AIzaSyDIegJ4BNrtNdYxTYSyUKcUPG-ploQfrNA"
#define FIREBASE_PROJECT_ID "cardiora"
#define COLLECTION_NAME "devices"

// ===== Device & Subcollection =====
const char* DEVICE_DOC = "LionDevice01";  // Document under 'devices'
const char* VALUES_COLLECTION = "SensorData"; // Subcollection for storing values

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 50000;  // 50 seconds

void setup() {
  // Initialize LionBit (LCD, buttons, etc.)
  lionStart();

  // ===== Connect WiFi =====
  ClsDis();
  TextDisplay("WiFi: Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  ClsDis();
  TextDisplay("WiFi Connected");
  delay(2000);

  // ===== Initialize Firebase =====
  config.api_key = API_KEY;                 
  Firebase.begin(&config, &auth);          
  Firebase.reconnectWiFi(true);

  // Anonymous sign-in
  if (Firebase.signUp(&config, &auth, "", "")) {
    ClsDis();
    TextDisplay("Firebase Ready");
    delay(2000);
  } else {
    ClsDis();
    TextDisplay("FB Sign-in Error");
    delay(3000);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastSendTime >= sendInterval) {
    lastSendTime = currentMillis;

    int randomValue = random(0, 101);
    unsigned long timestamp = millis(); // uptime as timestamp

    // Create JSON content for Firestore
    FirebaseJson content;
    content.set("fields/time/integerValue", String(timestamp));
    content.set("fields/value/integerValue", String(randomValue));

    // Build subcollection path: devices/LionDevice01/values
    String path = String(COLLECTION_NAME) + "/" + DEVICE_DOC + "/" + VALUES_COLLECTION;

    // Send data to Firestore
    if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", path, content.raw())) {
      ClsDis();
      String displayText = "Value Sent:\n" + String(randomValue);
      TextDisplay(displayText.c_str());
    } else {
      // Display exact Firebase error on LCD
      ClsDis();
      String errorText = "Send Failed:\n" + String(fbdo.errorReason().c_str());
      TextDisplay(errorText.c_str());
      delay(5000);  // Wait so user can read error
    }
  }
}
