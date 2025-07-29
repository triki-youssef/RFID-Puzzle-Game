#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D4
#define RST_PIN D3
#define LED_PIN D8
#define RESET_BUTTON D1
#define UDP_PORT 4210

const char* ssid = "RFID_GAME";
const char* password = "12345678";

WiFiUDP udp;
IPAddress clients[3];
bool received[4] = {false, false, false, false};  // 3 clients + central

MFRC522 mfrc522(SS_PIN, RST_PIN);
bool learnMode = false;
bool alreadyMatched = false;
byte storedUID[4] = {0x26, 0xFA, 0xF6, 0x04};  // Default UID for central

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_PIN, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  WiFi.softAP(ssid, password);
  udp.begin(UDP_PORT);
  Serial.println("Central AP + RFID Ready");
}

void loop() {
  // Handle UDP
  int packetSize = udp.parsePacket();
  if (packetSize) {
    IPAddress senderIP = udp.remoteIP();
    int readerID = udp.parseInt();
    int value = udp.parseInt();
    if (readerID >= 0 && readerID < 3 && value == 1) {
      received[readerID] = true;
      clients[readerID] = senderIP;
      Serial.printf("Client %d matched\n", readerID);
    }
  }

  // Central's RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte* uid = mfrc522.uid.uidByte;

    if (learnMode) {
      for (int i = 0; i < 4; i++) storedUID[i] = uid[i];
      learnMode = false;
      alreadyMatched = false;
      Serial.print("New UID for central stored: ");
      printUID(storedUID);
      delay(1000);  // give time to remove card
    } else if (!alreadyMatched && checkUID(uid)) {
      received[3] = true;
      alreadyMatched = true;
      Serial.println("Central card matched!");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  // Reset button
  if (digitalRead(RESET_BUTTON) == LOW) {
    resetAll();
    delay(500);
  }

  digitalWrite(LED_PIN, allMatched() ? HIGH : LOW);
}

bool checkUID(byte* uid) {
  for (int i = 0; i < 4; i++)
    if (uid[i] != storedUID[i]) return false;
  return true;
}

bool allMatched() {
  for (int i = 0; i < 4; i++)
    if (!received[i]) return false;
  return true;
}

void resetAll() {
  Serial.println("Resetting clients and central...");
  for (int i = 0; i < 3; i++) {
    received[i] = false;
    if (clients[i]) {
      udp.beginPacket(clients[i], UDP_PORT);
 when im using these 2 codes when i push reset i need to scan the cards in orders in learning mode ?
      udp.endPacket();
    }
  }
  received[3] = false;
  learnMode = true;
  alreadyMatched = false;
  mfrc522.PCD_Init();        // Reset RFID reader
  mfrc522.PCD_StopCrypto1();
  mfrc522.PICC_HaltA();
  Serial.println("Central entering learn mode...");
}

void printUID(byte* uid) {
  for (int i = 0; i < 4; i++) {
    Serial.print(uid[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}      #include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D4
#define RST_PIN D3
#define UDP_PORT 4210

const char* ssid = "RFID_GAME";
const char* password = "12345678";
IPAddress centralIP(192, 168, 4, 1);

WiFiUDP udp;
MFRC522 mfrc522(SS_PIN, RST_PIN);

int readerID = 0;  // CHANGE to 1 or 2 on other clients
bool alreadySent = false;
bool learnMode = false;
byte storedUID[4] = {0xDE, 0xAD, 0xBE, 0x01};  // Default UID

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  udp.begin(UDP_PORT);
  Serial.printf("Client %d ready\n", readerID);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char buf[10];
    udp.read(buf, 10);
    if (String(buf).startsWith("0")) {
      learnMode = true;
      alreadySent = false;
      Serial.printf("Client %d: Learn mode triggered\n", readerID);
    }
  }

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte* uid = mfrc522.uid.uidByte;

    if (learnMode) {
      for (int i = 0; i < 4; i++) storedUID[i] = uid[i];
      learnMode = false;
      alreadySent = false;
      Serial.print("New UID stored: ");
      printUID(storedUID);
      delay(1000);  // allow card removal
    } else if (!alreadySent && checkUID(uid)) {
      sendMatch();
      alreadySent = true;
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

bool checkUID(byte* uid) {
  for (int i = 0; i < 4; i++)
    if (uid[i] != storedUID[i]) return false;
  return true;
}

void sendMatch() {
  udp.beginPacket(centralIP, UDP_PORT);
  udp.printf("%d 1", readerID);
  udp.endPacket();
  Serial.printf("Client %d sent match\n", readerID);
}

void printUID(byte* uid) {
  for (int i = 0; i < 4; i++) {
    Serial.print(uid[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}