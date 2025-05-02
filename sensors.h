#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"
#include "notification_manager.h"
#include "firebase_manager.h"

// Forward declare functions to avoid circular reference
void handleIntrusion(int ldrValue);
void sendAlertsAndLogs(int ldrValue);

// Initialize system variables
unsigned long lastAlertTime = 0; // Time of last alert
unsigned long lastLaserCheckTime = 0; // Time of last laser check
int debounceDelay = 500; // Minimum time between laser checks (in milliseconds)

void setupSensors() {
  // Initialize pins
  pinMode(LASER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  
  // Initial states
  digitalWrite(LASER_PIN, HIGH); // Turn laser on
  digitalWrite(BUZZER_PIN, LOW); // Turn buzzer off
  
  Serial.println("Sensors initialized");
}

void setLaserState(bool state) {
  digitalWrite(LASER_PIN, state);
}

void setBuzzerState(bool state) {
  digitalWrite(BUZZER_PIN, state);
}

void checkLaserStatus() {
  unsigned long currentTime = millis();
  
  // Check every debounceDelay milliseconds to prevent spamming alerts
  if (currentTime - lastLaserCheckTime >= debounceDelay) {
    lastLaserCheckTime = currentTime;
    int ldrValue = analogRead(LDR_PIN);
    
    #ifdef DEBUG
      Serial.print("LDR Value: ");
      Serial.println(ldrValue);
      Serial.print(F("Free heap: "));
      Serial.println(ESP.getFreeHeap());
    #endif
    
    // If light level is below threshold (laser is blocked)
    if (ldrValue < ldrThreshold) {
      handleIntrusion(ldrValue);
    } else {
      // Reset alarm if light level is back to normal
      if (alarmActive) {
        alarmActive = false;
        setBuzzerState(LOW);
      }
    }
  }
}

void handleIntrusion(int ldrValue) {
  unsigned long currentTime = millis();
  
  // Activate buzzer and process only if alert cooldown period has passed
  if (!alarmActive) {
    alarmActive = true;
    setBuzzerState(HIGH);
    
    // Send notifications and log event after cooldown
    if (currentTime - lastAlertTime > alertCooldown) {
      lastAlertTime = currentTime;
      sendAlertsAndLogs(ldrValue);
    }
  }
}

void sendAlertsAndLogs(int ldrValue) {
  // Send notifications (Telegram, Blynk, etc.)
  sendTelegramAlert(ldrValue);
  sendBlynkAlert();
  
  // Log the intrusion event to Firebase
  logIntrusionEvent(ldrValue);
}

void updateSystemStatus() {
  // Check if the system is disarmed and reset alarm if necessary
  if (!systemArmed && alarmActive) {
    alarmActive = false;
    setBuzzerState(LOW);
  }
}

#endif // SENSORS_H
