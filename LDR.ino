#include "blynk_config.h"

// Include all necessary libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleEsp8266.h>
#include <UniversalTelegramBot.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Include our custom header files
#include "config.h"
#include "firebase_manager.h"
#include "data_manager.h"
#include "wifi_manager.h"
#include "sensors.h"
#include "notification_manager.h"
#include "web_interface.h"

// Define global variables for time and retry delays
unsigned long previousMillis = 0;
const long interval = 1000;  // Update interval for checking sensors
bool systemArmed = false;   // Definition of systemArmed
bool alarmActive = false;   // Definition of alarmActive


void setup() {
  Serial.begin(115200);
  Serial.println("\nESP8266 Laser Security System Starting...");
  Serial.println("User: varunkainth");
  
  // Initialize components
  setupWiFi();
  setupSensors();
  setupNTP();
  setupNotifications();
  setupWebServer();

  Serial.println("System initialization complete!");
}

void loop() {
  // Non-blocking time-based check using millis()
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the time
    if (systemArmed) {
      checkLaserStatus(); // Check sensor status only if the system is armed
    }
  }

  // Handle external interactions and Blynk commands
  handleTelegramCommands();
  Blynk.run();
  server.handleClient();
  timeClient.update();
}

// Blynk handlers
BLYNK_WRITE(V0) {
  systemArmed = param.asInt(); // Toggle system armed state
  updateSystemStatus();
}

BLYNK_WRITE(V1) {
  setLaserState(param.asInt()); // Toggle laser state
}

BLYNK_WRITE(V2) {
  setBuzzerState(param.asInt()); // Toggle buzzer state
}
