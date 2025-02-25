// #include <Arduino.h>
// #include <ESP32Servo.h>
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>


// const char* ssid = "Wokwi-GUEST";    
// const char* password = "";           

// const char* scriptURL = "https://script.google.com/macros/s/AKfycbxlmRvEpsYV7X7H3gU3vPFicqfryLE5i7lqRP_KF8jk5EUF7vgpwEVMf1KU0Mdn_5G-/exec";


// const int btnInc1 = 22, btnDec1 = 23, ledGreen1 = 21, ledRed1 = 19, servoPin1 = 18;

// const int btnInc2 = 16, btnDec2 = 17, ledGreen2 = 15, ledRed2 = 2, servoPin2 = 5;

// const int btnInc3 = 0, btnDec3 = 4, ledGreen3 = 33, ledRed3 = 32, servoPin3 = 13;


// Servo waterValve1, waterValve2, waterValve3;


// int waterBalance1 = 5, waterBalance2 = 5, waterBalance3 = 5;


// unsigned long lastPress1 = 0, lastPress2 = 0, lastPress3 = 0;
// const unsigned long debounceDelay = 300;

// void connectWiFi() {
//     WiFi.begin(ssid, password);
//     Serial.print("Connecting to WiFi...");
//     int attempts = 0;
//     while (WiFi.status() != WL_CONNECTED && attempts < 20) {
//         delay(500);
//         Serial.print(".");
//         attempts++;
//     }
//     Serial.println(WiFi.status() == WL_CONNECTED ? "\n✅ WiFi Connected!" : "\n❌ WiFi Connection Failed!");
// }


// void updateLEDs() {
//     digitalWrite(ledGreen1, waterBalance1 > 0 ? HIGH : LOW);
//     digitalWrite(ledRed1, waterBalance1 == 0 ? HIGH : LOW);

//     digitalWrite(ledGreen2, waterBalance2 > 0 ? HIGH : LOW);
//     digitalWrite(ledRed2, waterBalance2 == 0 ? HIGH : LOW);

//     digitalWrite(ledGreen3, waterBalance3 > 0 ? HIGH : LOW);
//     digitalWrite(ledRed3, waterBalance3 == 0 ? HIGH : LOW);
// }


// void sendToSheet(int house, int balance) {
//     if (WiFi.status() != WL_CONNECTED) {
//         Serial.println("⚠️ WiFi Disconnected. Skipping data send.");
//         return;
//     }

//     HTTPClient http;
//     http.begin(scriptURL);
//     http.addHeader("Content-Type", "application/json");

//     DynamicJsonDocument jsonDoc(200);
//     jsonDoc["house"] = house;
//     jsonDoc["balance"] = balance;

//     String jsonPayload;
//     serializeJson(jsonDoc, jsonPayload);

//     int httpResponseCode = http.POST(jsonPayload);
//     Serial.printf("📡 House %d - HTTP Response: %d\n", house, httpResponseCode);

//     http.end();
// }


// void fetchBalance(int houseNumber, int &balance) {
//     if (WiFi.status() != WL_CONNECTED) {
//         Serial.println("⚠️ WiFi Disconnected. Skipping balance fetch.");
//         return;
//     }

//     HTTPClient http;
//     String url = String(scriptURL) + "?house=" + String(houseNumber);
//     http.begin(url);
    
//     int httpResponseCode = http.GET();
//     if (httpResponseCode > 0) {
//         String payload = http.getString();
//         DynamicJsonDocument jsonDoc(200);
//         deserializeJson(jsonDoc, payload);
//         balance = jsonDoc["balance"];
//         Serial.printf("✅ House %d Balance: %d\n", houseNumber, balance);
//     } else {
//         Serial.printf("⚠️ HTTP GET Failed for House %d. Code: %d\n", houseNumber, httpResponseCode);
//     }

//     http.end();
// }


// void dispenseWater(Servo &waterValve, int houseNumber) {
//     waterValve.write(0);
//     Serial.printf("⏳ House %d - Dispensing Water...\n", houseNumber);
    
//     delay(3000);

//     waterValve.write(90);
//     Serial.println("❌ Valve Closed.");
// }


// void handleHouse(int btnInc, int btnDec, int &waterBalance, Servo &waterValve, int houseNumber, unsigned long &lastPress) {
//     unsigned long currentTime = millis();

//     if (digitalRead(btnInc) == LOW && (currentTime - lastPress > debounceDelay)) {  
//         lastPress = currentTime;
//         waterBalance++;
//         Serial.printf("🏠 House %d - Balance Increased: %d\n", houseNumber, waterBalance);
//         sendToSheet(houseNumber, waterBalance);
//     }

//     if (digitalRead(btnDec) == LOW && waterBalance > 0 && (currentTime - lastPress > debounceDelay)) {  
//         lastPress = currentTime;
//         waterBalance--;
//         Serial.printf("🏠 House %d - Balance Decreased: %d\n", houseNumber, waterBalance);
//         sendToSheet(houseNumber, waterBalance);
//         dispenseWater(waterValve, houseNumber);
//     }

//     updateLEDs();
// }

// void setup() {
//     Serial.begin(115200);
//     connectWiFi();

    
//     pinMode(btnInc1, INPUT_PULLUP);
//     pinMode(btnDec1, INPUT_PULLUP);
//     pinMode(ledGreen1, OUTPUT);
//     pinMode(ledRed1, OUTPUT);
//     waterValve1.attach(servoPin1);
//     waterValve1.write(90);

    
//     pinMode(btnInc2, INPUT_PULLUP);
//     pinMode(btnDec2, INPUT_PULLUP);
//     pinMode(ledGreen2, OUTPUT);
//     pinMode(ledRed2, OUTPUT);
//     waterValve2.attach(servoPin2);
//     waterValve2.write(90);

    
//     pinMode(btnInc3, INPUT_PULLUP);
//     pinMode(btnDec3, INPUT_PULLUP);
//     pinMode(ledGreen3, OUTPUT);
//     pinMode(ledRed3, OUTPUT);
//     waterValve3.attach(servoPin3);
//     waterValve3.write(90);

//     Serial.println("✅ System Initialized!");
//     updateLEDs();
// }

// void loop() {
//     handleHouse(btnInc1, btnDec1, waterBalance1, waterValve1, 1, lastPress1);
//     handleHouse(btnInc2, btnDec2, waterBalance2, waterValve2, 2, lastPress2);
//     handleHouse(btnInc3, btnDec3, waterBalance3, waterValve3, 3, lastPress3);
// }
















#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <RTClib.h>  // Include RTC library

const char* ssid = "Wokwi-GUEST";    
const char* password = "";           

const char* scriptURL = "https://script.google.com/macros/s/AKfycbwQsgYdHIvyLdDoF5VPlIOZKaBqSmriqKNfaaSkZEib52G4ICz3Pb7FmHc5sVAB8gSZJQ/exec";

// Pin configuration
const int btnInc1 = 26, btnDec1 = 23, ledGreen1 = 25, ledRed1 = 19, servoPin1 = 18;
const int btnInc2 = 16, btnDec2 = 17, ledGreen2 = 15, ledRed2 = 2, servoPin2 = 5;
const int btnInc3 = 0, btnDec3 = 4, ledGreen3 = 33, ledRed3 = 32, servoPin3 = 13;

Servo waterValve1, waterValve2, waterValve3;

int waterBalance1 = 0, waterBalance2 = 0, waterBalance3 = 0; // Initializing balance to zero
int dailyLimit1 = 0, dailyLimit2 = 0, dailyLimit3 = 0; // Initial daily limits (to be fetched)
unsigned long lastPress1 = 0, lastPress2 = 0, lastPress3 = 0;
const unsigned long debounceDelay = 300;

RTC_DS3231 rtc;  // RTC object

const int maxRetries = 5; // Maximum retry attempts

void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print("...");
        attempts++;
    }
    Serial.println(WiFi.status() == WL_CONNECTED ? "\n✅ WiFi Connected!" : "\n❌ WiFi Connection Failed!");
}

// Function to update LEDs based on water balance
void updateLEDs() {
    digitalWrite(ledGreen1, waterBalance1 > 0 ? HIGH : LOW);
    digitalWrite(ledRed1, waterBalance1 == 0 ? HIGH : LOW);
    digitalWrite(ledGreen2, waterBalance2 > 0 ? HIGH : LOW);
    digitalWrite(ledRed2, waterBalance2 == 0 ? HIGH : LOW);
    digitalWrite(ledGreen3, waterBalance3 > 0 ? HIGH : LOW);
    digitalWrite(ledRed3, waterBalance3 == 0 ? HIGH : LOW);
}

// Function to send data to Google Sheets
void sendToSheet(int house, int balance, int dailyLimit) {
    int attempt = 0;
    bool success = false;

    while (attempt < maxRetries && !success) {
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
        jsonDoc["dailyLimit"] = dailyLimit;

        String jsonPayload;
        serializeJson(jsonDoc, jsonPayload);

        int httpResponseCode = http.POST(jsonPayload);
        if (httpResponseCode > 0) {
            Serial.printf("📡 House %d - HTTP Response: %d\n", house, httpResponseCode);
            success = true;
        } else {
            Serial.printf("⚠️ Failed to send data for House %d. Retry %d of %d\n", house, attempt + 1, maxRetries);
            attempt++;
            delay(2000); // Wait before retrying
        }

        http.end();
    }

    if (!success) {
        Serial.println("❌ Failed to send data after retries.");
    }
}

// Function to fetch balance and daily limit for a house
void fetchBalanceAndLimit(int houseNumber, int &balance, int &dailyLimit) {
    int attempt = 0;
    bool success = false;

    while (attempt < maxRetries && !success) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("⚠️ WiFi Disconnected. Skipping balance and limit fetch.");
            return;
        }

        HTTPClient http;
        String url = String(scriptURL) + "?house=" + String(houseNumber);
        http.begin(url.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

        int httpResponseCode = http.GET();
        Serial.println(httpResponseCode);

        if (httpResponseCode > 0) {
            String payload = http.getString();
            Serial.println("Response received: " + payload);  // Debugging the response
            
            DynamicJsonDocument jsonDoc(1024);
            deserializeJson(jsonDoc, payload);

            // if (error) {
            //     Serial.println("❌ JSON Deserialization failed!");
            //     Serial.println(error.c_str());
            //     attempt++;
            //     delay(2000); // Wait before retrying
            //     continue;
            // }

            // Check if keys exist before extracting values
            if (jsonDoc.containsKey("balance") && jsonDoc.containsKey("dailyLimit")) {
                balance = jsonDoc["balance"];
                dailyLimit = jsonDoc["dailyLimit"];
                Serial.printf("✅ House %d - Balance: %d, Daily Limit: %d\n", houseNumber, balance, dailyLimit);
                success = true;
            } else {
                Serial.println("❌ Keys 'balance' or 'dailyLimit' not found in response.");
                attempt++;
                delay(2000); // Wait before retrying
            }

        } else {
            Serial.printf("⚠️ HTTP request failed. Response code: %d\n", httpResponseCode);
            attempt++;
            delay(2000); // Wait before retrying
        }

        http.end();
    }

    if (!success) {
        Serial.println("❌ Failed to fetch data after retries.");
    }
}


// Function to dispense water
void dispenseWater(Servo &waterValve, int houseNumber) {
    waterValve.write(0);
    Serial.printf("⏳ House %d - Dispensing Water...\n", houseNumber);
    
    delay(3000);

    waterValve.write(90);
    Serial.println("❌ Valve Closed.");
}

// Function to handle each house (inc/dec button, water valve, etc.)
void handleHouse(int btnInc, int btnDec, int &waterBalance, int &dailyLimit, Servo &waterValve, int houseNumber, unsigned long &lastPress) {
    unsigned long currentTime = millis();

    if (digitalRead(btnInc) == LOW && (currentTime - lastPress > debounceDelay)) {  
        lastPress = currentTime;
        if (waterBalance < dailyLimit) {
            waterBalance++;
            Serial.printf("🏠 House %d - Balance Increased: %d\n", houseNumber, waterBalance);
            sendToSheet(houseNumber, waterBalance, dailyLimit);
        } else {
            Serial.println("⚠️ Daily limit reached. Cannot increase balance.");
        }
    }

    if (digitalRead(btnDec) == LOW && waterBalance > 0 && (currentTime - lastPress > debounceDelay)) {  
        lastPress = currentTime;
        waterBalance--;
        Serial.printf("🏠 House %d - Balance Decreased: %d\n", houseNumber, waterBalance);
        sendToSheet(houseNumber, waterBalance, dailyLimit);
        dispenseWater(waterValve, houseNumber);
    }

    updateLEDs();
}

// Setup function to initialize the system
void setup() {
    Serial.begin(115200);
    connectWiFi();

    if (!rtc.begin()) {
        Serial.println("❌ Couldn't find RTC");
        while (1);
    }

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

    // Fetch initial balance and daily limits for each house
    fetchBalanceAndLimit(1, waterBalance1, dailyLimit1);
    fetchBalanceAndLimit(2, waterBalance2, dailyLimit2);
    fetchBalanceAndLimit(3, waterBalance3, dailyLimit3);

    Serial.println("✅ System Initialized!");
    updateLEDs();
}

// Main loop to handle all house operations
void loop() {
    handleHouse(btnInc1, btnDec1, waterBalance1, dailyLimit1, waterValve1, 1, lastPress1);
    handleHouse(btnInc2, btnDec2, waterBalance2, dailyLimit2, waterValve2, 2, lastPress2);
    handleHouse(btnInc3, btnDec3, waterBalance3, dailyLimit3, waterValve3, 3, lastPress3);

    // Print current time every 10 seconds (for debugging)
    DateTime now = rtc.now();
    // Serial.printf("Current Time: %02d:%02d:%02d\n", now.hour(), now.minute(), now.second());
    // delay(10000);
}








// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>


// const char* ssid = "Wokwi-GUEST";
// const char* password = "";
// String url = "https://script.google.com/macros/s/AKfycbwQsgYdHIvyLdDoF5VPlIOZKaBqSmriqKNfaaSkZEib52G4ICz3Pb7FmHc5sVAB8gSZJQ/exec?house=1";

// void setup() {
//   Serial.begin(115200);
//   delay(10);

//   // Connect to Wi-Fi
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }
//   Serial.println("WiFi connected");

//   // Make the HTTP request
//   HTTPClient http;

//   // HTTP GET Request.
//       http.begin(url.c_str());
//       http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

//       // Gets the HTTP status code.
//       int httpCode = http.GET(); 
//       Serial.print("HTTP Status Code : ");
//       Serial.println(httpCode);
  
//       // Getting response from google sheet.
//       String payload;
//       if (httpCode > 0) {
//         payload = http.getString();
//         Serial.println("Payload : " + payload);  
//       }
//   // http.begin(url); // Specify the URL
//   // int httpCode = http.GET(); // Make the GET request
  
//   // Check if the request was successful
//   //if (httpCode == 200) {
//     // String payload = http.getString();
//     // Serial.println("Response received:");
//     // Serial.println(payload);

//     // Parse the JSON response
//     DynamicJsonDocument doc(1024);
//     deserializeJson(doc, payload);

//     // Extract balance and dailyLimit
//     int balance = doc["balance"];
//     int dailyLimit = doc["dailyLimit"];

//     Serial.print("Balance: ");
//     Serial.println(balance);
//     Serial.print("Daily Limit: ");
//     Serial.println(dailyLimit);
    
//     // } else {
//   //  Serial.println("Failed to retrieve data");
//   // }

//   http.end(); // End the HTTP request
// }

// void loop() {
//   // Nothing to do here for now
// }