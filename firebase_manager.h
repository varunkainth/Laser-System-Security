#ifndef FIREBASE_MANAGER_H
#define FIREBASE_MANAGER_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "config.h"
#include "data_manager.h"

String getFormattedDateTime(); // Forward declaration

// Debug flag for memory tracking (enable in config.h if desired)
void printHeap(const String &label) {
  #if DEBUG_HEAP
    Serial.print("[DEBUG] ");
    Serial.print(label);
    Serial.print(" - Free heap: ");
    Serial.println(ESP.getFreeHeap());
  #endif
}

// Store data to Firebase via HTTPS POST
bool storeDataToFirebase(const String &path, const String &jsonData) {
  WiFiClientSecure client;
  client.setInsecure();  // Skip certificate verification to save memory

  String fullURL = String("https://") + FIREBASE_HOST + path + "?auth=" + DATABASE_SECRET;

  printHeap("Before HTTP POST");

  HTTPClient http;
  if (!http.begin(client, fullURL)) {
    Serial.println("❌ HTTP begin failed!");
    return false;
  }

  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(jsonData);

  if (httpCode > 0) {
    Serial.println("✅ Data stored to Firebase. Code: " + String(httpCode));
    Serial.println("Response: " + http.getString());
  } else {
    Serial.println("❌ Failed to send data: " + http.errorToString(httpCode));
  }

  http.end();
  printHeap("After HTTP POST");

  return httpCode > 0;
}

// Log intrusion event to Firebase
void logIntrusionEvent(int ldrValue) {
  Serial.println("Logging intrusion event via HTTPS...");

  String formattedDateTime = getFormattedDateTime();
  String dateOnly = formattedDateTime.substring(0, 10);
  String epoch = String(timeClient.getEpochTime());

  String firebasePath = "/laser_events/" + dateOnly + "/" + epoch + ".json";

  // Build JSON using ArduinoJson
  StaticJsonDocument<256> doc;
  doc["timestamp"] = formattedDateTime;
  doc["date"] = dateOnly;
  doc["time"] = formattedDateTime.substring(11);
  doc["ldrValue"] = ldrValue;
  doc["status"] = "intrusion";
  doc["user"] = "varunkainth";

  String payload;
  serializeJson(doc, payload);

  storeDataToFirebase(firebasePath, payload);
}

// Fetch events for a specific date
String getEventsForDate(const String &date) {
  WiFiClientSecure client;
  client.setInsecure();

  String url = String("https://") + FIREBASE_HOST + "/laser_events/" + date + ".json?auth=" + DATABASE_SECRET;

  printHeap("Before GET date events");

  HTTPClient http;
  if (!http.begin(client, url)) {
    Serial.println("❌ HTTP begin failed!");
    return "{}";
  }

  int httpCode = http.GET();
  String response = "{}";

  if (httpCode > 0) {
    response = http.getString();
    Serial.println("✅ Events fetched for " + date);
  } else {
    Serial.println("❌ Failed to fetch events: " + http.errorToString(httpCode));
  }

  http.end();
  printHeap("After GET date events");

  return response;
}

// Fetch all events
String getAllEvents() {
  WiFiClientSecure client;
  client.setInsecure();

  String url = String("https://") + FIREBASE_HOST + "/laser_events.json?auth=" + DATABASE_SECRET;

  printHeap("Before GET all events");

  HTTPClient http;
  if (!http.begin(client, url)) {
    Serial.println("❌ HTTP begin failed!");
    return "{}";
  }

  int httpCode = http.GET();
  String response = "{}";

  if (httpCode > 0) {
    response = http.getString();
    Serial.println("✅ All events fetched");
  } else {
    Serial.println("❌ Failed to fetch all events: " + http.errorToString(httpCode));
  }

  http.end();
  printHeap("After GET all events");

  return response;
}

// Clear all stored events
bool clearAllAlarms() {
  WiFiClientSecure client;
  client.setInsecure();

  String url = String("https://") + FIREBASE_HOST + "/laser_events.json?auth=" + DATABASE_SECRET;

  printHeap("Before DELETE alarms");

  HTTPClient http;
  if (!http.begin(client, url)) {
    Serial.println("❌ HTTP begin failed!");
    return false;
  }

  int httpCode = http.sendRequest("DELETE");

  if (httpCode == 200) {
    Serial.println("✅ All alarms cleared.");
  } else {
    Serial.println("❌ Failed to clear alarms: " + http.errorToString(httpCode));
  }

  http.end();
  printHeap("After DELETE alarms");

  return httpCode == 200;
}

// Format timestamp
String getFormattedDateTime() {
  timeClient.update();
  time_t rawtime = timeClient.getEpochTime();
  struct tm* ti = localtime(&rawtime);

  char buffer[30];
  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
           ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday,
           ti->tm_hour, ti->tm_min, ti->tm_sec);
  return String(buffer);
}

#endif // FIREBASE_MANAGER_H
