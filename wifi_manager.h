#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "config.h"  // Include config.h to access constants and global variables

// Declare global objects
ESP8266WebServer server(80);
WiFiClientSecure secured_client;  // Secure client for Telegram and Firebase
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Initialize system variables (from config.h)
extern bool systemArmed;        // Accessing global variable declared in config.h
extern bool alarmActive;        // Accessing global variable declared in config.h
extern unsigned long lastAlertTime; // Accessing global variable declared in config.h

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  secured_client.setInsecure(); // For ESP8266 to accept all certs
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost connection. Reconnecting...");
    setupWiFi();
  }
}

void setupNTP() {
  timeClient.begin();
  timeClient.setTimeOffset(0); // Set your timezone offset in seconds
  Serial.println("NTP client initialized");
}

void checkNTP() {
  if (!timeClient.update()) {
    Serial.println("Failed to get NTP time. Retrying...");
    timeClient.forceUpdate(); // Force update if NTP sync fails
  }
}

#endif // WIFI_MANAGER_H
