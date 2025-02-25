#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char* ssid = "Wokwi-GUEST";    
const char* password = "";           
const char* scriptURL = "https://script.google.com/macros/s/AKfycbwXihdQo5udDyJoz1s6HHtwg-Tv3PlAp8XKk2YQelBwfbLZ0EgQbR7OC2d6708FWFYZVQ/exec";


const int btnInc1 = 22, btnDec1 = 23, ledGreen1 = 21, ledRed1 = 19, servoPin1 = 18;

const int btnInc2 = 16, btnDec2 = 17, ledGreen2 = 15, ledRed2 = 2, servoPin2 = 5;

const int btnInc3 = 0, btnDec3 = 4, ledGreen3 = 33, ledRed3 = 32, servoPin3 = 13;


Servo waterValve1, waterValve2, waterValve3;


int waterBalance1 = 5, waterBalance2 = 5, waterBalance3 = 5;


unsigned long lastPress1 = 0, lastPress2 = 0, lastPress3 = 0;
const unsigned long debounceDelay = 300;

void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    Serial.println(WiFi.status() == WL_CONNECTED ? "\n✅ WiFi Connected!" : "\n❌ WiFi Connection Failed!");
}


void updateLEDs() {
    digitalWrite(ledGreen1, waterBalance1 > 0 ? HIGH : LOW);
    digitalWrite(ledRed1, waterBalance1 == 0 ? HIGH : LOW);

    digitalWrite(ledGreen2, waterBalance2 > 0 ? HIGH : LOW);
    digitalWrite(ledRed2, waterBalance2 == 0 ? HIGH : LOW);

    digitalWrite(ledGreen3, waterBalance3 > 0 ? HIGH : LOW);
    digitalWrite(ledRed3, waterBalance3 == 0 ? HIGH : LOW);
}


void sendToSheet(int house, int balance) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("⚠️ WiFi Disconnected. Skipping data send.");
        return;
    }

    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument jsonDoc(200);
    jsonDoc["house"] = house;
    jsonDoc["balance"] = balance;

    String jsonPayload;
    serializeJson(jsonDoc, jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);
    Serial.printf("📡 House %d - HTTP Response: %d\n", house, httpResponseCode);

    http.end();
}


void fetchBalance(int houseNumber, int &balance) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("⚠️ WiFi Disconnected. Skipping balance fetch.");
        return;
    }

    HTTPClient http;
    String url = String(scriptURL) + "?house=" + String(houseNumber);
    http.begin(url);
    
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
        String payload = http.getString();
        DynamicJsonDocument jsonDoc(200);
        deserializeJson(jsonDoc, payload);
        balance = jsonDoc["balance"];
        Serial.printf("✅ House %d Balance: %d\n", houseNumber, balance);
    } else {
        Serial.printf("⚠️ HTTP GET Failed for House %d. Code: %d\n", houseNumber, httpResponseCode);
    }

    http.end();
}


void dispenseWater(Servo &waterValve, int houseNumber) {
    waterValve.write(0);
    Serial.printf("⏳ House %d - Dispensing Water...\n", houseNumber);
    
    delay(3000);

    waterValve.write(90);
    Serial.println("❌ Valve Closed.");
}


void handleHouse(int btnInc, int btnDec, int &waterBalance, Servo &waterValve, int houseNumber, unsigned long &lastPress) {
    unsigned long currentTime = millis();

    if (digitalRead(btnInc) == LOW && (currentTime - lastPress > debounceDelay)) {  
        lastPress = currentTime;
        waterBalance++;
        Serial.printf("🏠 House %d - Balance Increased: %d\n", houseNumber, waterBalance);
        sendToSheet(houseNumber, waterBalance);
    }

    if (digitalRead(btnDec) == LOW && waterBalance > 0 && (currentTime - lastPress > debounceDelay)) {  
        lastPress = currentTime;
        waterBalance--;
        Serial.printf("🏠 House %d - Balance Decreased: %d\n", houseNumber, waterBalance);
        sendToSheet(houseNumber, waterBalance);
        dispenseWater(waterValve, houseNumber);
    }

    updateLEDs();
}

void setup() {
    Serial.begin(115200);
    connectWiFi();

    
    pinMode(btnInc1, INPUT_PULLUP);
    pinMode(btnDec1, INPUT_PULLUP);
    pinMode(ledGreen1, OUTPUT);
    pinMode(ledRed1, OUTPUT);
    waterValve1.attach(servoPin1);
    waterValve1.write(90);

    
    pinMode(btnInc2, INPUT_PULLUP);
    pinMode(btnDec2, INPUT_PULLUP);
    pinMode(ledGreen2, OUTPUT);
    pinMode(ledRed2, OUTPUT);
    waterValve2.attach(servoPin2);
    waterValve2.write(90);

    
    pinMode(btnInc3, INPUT_PULLUP);
    pinMode(btnDec3, INPUT_PULLUP);
    pinMode(ledGreen3, OUTPUT);
    pinMode(ledRed3, OUTPUT);
    waterValve3.attach(servoPin3);
    waterValve3.write(90);

    Serial.println("✅ System Initialized!");
    updateLEDs();
}

void loop() {
    handleHouse(btnInc1, btnDec1, waterBalance1, waterValve1, 1, lastPress1);
    handleHouse(btnInc2, btnDec2, waterBalance2, waterValve2, 2, lastPress2);
    handleHouse(btnInc3, btnDec3, waterBalance3, waterValve3, 3, lastPress3);
}