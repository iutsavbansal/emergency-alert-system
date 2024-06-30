/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

// ESP32 #2: TCP SERVER + AN LED
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif
#include <LiquidCrystal.h>


LiquidCrystal lcd(D8, D5, D4, D3, D2, D1);


#define LED_PIN  D7 // ESP32 pin GPIO18 connected to LED
#define SERVER_PORT 4080

const char* ssid = "Utsav-PC";     // CHANGE TO YOUR WIFI SSID
const char* password = "utsavbansal"; // CHANGE TO YOUR WIFI PASSWORD

WiFiServer TCPserver(SERVER_PORT);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();

  Serial.println("ESP32 #2: TCP SERVER + AN LED");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print your local IP address:
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

  // Start listening for a TCP client (from ESP32 #1)
  TCPserver.begin();
}

void loop() {
  // Wait for a TCP client from ESP32 #1:
  WiFiClient client = TCPserver.available();

  if (client) {
    // Read the command from the TCP client:
    char command = client.read();
    Serial.print("ESP32 #2: - Received command: ");
    Serial.println(command);

    if (command == '1'){
      digitalWrite(LED_PIN, HIGH); // Turn LED on
        lcd.print("Emergency in");
        lcd.setCursor(0, 1);
        lcd.print("Electrical Lab");
    }

    else if (command == '0'){
      digitalWrite(LED_PIN, LOW);  // Turn LED off
      lcd.clear();
      client.stop();
  }
}
}
