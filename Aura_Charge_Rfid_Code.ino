#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 15
#define RST_PIN 0
#define LED_PIN 2        // NodeMCU D4 (active LOW)

#define COST_PER_SECOND 0.105   // ₹0.105 per second

// ------------------------
//  RFID → NAME MAPPING
// ------------------------
struct User {
  String uid;
  String name;
};

User users[] = {
  {"04765B2A651990", "Manit Shukla"},
  {"046E5B2A651990", "Shivam Roy"},
  {"046C5A2A651990", "Enter_your_name"}
};

int totalUsers = sizeof(users) / sizeof(users[0]);

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Charging variables
bool chargingState = false;
unsigned long chargingStartTime = 0;

String getNameFromUID(String uid) {
  for (int i = 0; i < totalUsers; i++) {
    if (users[i].uid == uid) {
      return users[i].name;
    }
  }
  return "Unknown User";
}

void clearSerialMonitor() {
  // ANSI Terminal Clear Command
  Serial.print("\033[2J\033[H");

  // Backup clearing if ANSI not supported
  Serial.println("\n\n\n\n\n\n\n\n\n\n");
}

void toggleLED(bool state) {
  digitalWrite(LED_PIN, state ? LOW : HIGH);  // LOW = ON
}

String formatTime(unsigned long seconds) {
  int hrs = seconds / 3600;
  seconds %= 3600;
  int mins = seconds / 60;
  int secs = seconds % 60;

  char buffer[20];
  sprintf(buffer, "%02d:%02d:%02d", hrs, mins, secs);
  return String(buffer);
}

void setup() {
  Serial.begin(74880);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // OFF initially

  Serial.println("RFID Charging System Ready...");
}

void loop() {

  if (!mfrc522.PICC_IsNewCardPresent() || 
      !mfrc522.PICC_ReadCardSerial()) {
    delay(100);
    return;
  }

  // Read UID
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  String name = getNameFromUID(uid);

  // ------------ CHARGING LOGIC -------------
  if (!chargingState) {

    // Clear screen before showing new charging data
    clearSerialMonitor();

    Serial.println("----------------------------------");
    Serial.print("Card Scanned UID : ");
    Serial.println(uid);
    Serial.print("Card Owner       : ");
    Serial.println(name);

    // Start charging
    chargingState = true;
    chargingStartTime = millis() / 1000; // seconds
    toggleLED(true);

    Serial.println("Charging Start");
    Serial.println("----------------------------------");
  } 
  else {

    Serial.println("----------------------------------");
    Serial.print("Card Scanned UID : ");
    Serial.println(uid);
    Serial.print("Card Owner       : ");
    Serial.println(name);

    // Stop charging
    chargingState = false;
    toggleLED(false);

    unsigned long chargingEndTime = millis() / 1000;
    unsigned long duration = chargingEndTime - chargingStartTime;

    Serial.println("Charging End");

    // Time display
    Serial.print("Total Charging Time : ");
    Serial.println(formatTime(duration));   // HH:MM:SS

    // Cost calculation
    float cost = duration * COST_PER_SECOND;

    Serial.print("Charging Cost (₹)   : ");
    Serial.println(cost, 2);    // 2 decimal places

    Serial.println("----------------------------------");
  }

  delay(1200);
}
