#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "config.h"
#include "blynk_config.h"

// Secure WiFi client for Telegram
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Declare the global variables for Telegram checking interval
unsigned long lastTelegramCheck = 0;

// Function to allow all certificates (insecure, for dev only)
void trustAllCertificates() {
  secured_client.setInsecure(); // Accept all certs (dev only)
}

// Function to initialize notification systems (Blynk, Telegram)
void setupNotifications() {
  trustAllCertificates();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password); // Initialize Blynk
  Serial.println("Blynk initialized");

  // Set up pin modes for laser and buzzer
  pinMode(LASER_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Set initial states
  digitalWrite(LASER_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Notification systems initialized");
}

// Function to send Telegram alert with LDR value
void sendTelegramAlert(int ldrValue) {
  String message = "⚠️ *ALERT: Laser beam interrupted!*\nLDR value: `" + String(ldrValue) + "`";
  bot.sendMessage(CHAT_ID, message, "Markdown"); // Send alert to chat
}

// Function to send alert to Blynk app
void sendBlynkAlert() {
  Blynk.logEvent("laser_interrupted", "Security breach detected! Laser beam interrupted.");
}

// Function to handle incoming Telegram commands (laser/buzzer control)
void handleTelegramCommands() {
  if (millis() - lastTelegramCheck > telegramCheckInterval) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        String text = bot.messages[i].text;
        String chat_id = bot.messages[i].chat_id;

        // Check if the user is authorized (for example, by checking chat_id)
        if (chat_id != AUTHORIZED_CHAT_ID) {
          bot.sendMessage(chat_id, "❌ You are not authorized to control this device.");
          continue;
        }

        // Handle different bot commands
        if (text == "/laser on") {
          digitalWrite(LASER_PIN, HIGH); // Turn laser on
          bot.sendMessage(chat_id, "🔆 Laser turned ON");
        } 
        else if (text == "/laser off") {
          digitalWrite(LASER_PIN, LOW);  // Turn laser off
          bot.sendMessage(chat_id, "💡 Laser turned OFF");
        } 
        else if (text == "/buzzer on") {
          digitalWrite(BUZZER_PIN, HIGH); // Turn buzzer on
          bot.sendMessage(chat_id, "🔊 Buzzer turned ON");
        } 
        else if (text == "/buzzer off") {
          digitalWrite(BUZZER_PIN, LOW);  // Turn buzzer off
          bot.sendMessage(chat_id, "🔕 Buzzer turned OFF");
        }
        else if (text == "/all on") {
          digitalWrite(LASER_PIN, HIGH);
          digitalWrite(BUZZER_PIN, HIGH); // Turn both on
          bot.sendMessage(chat_id, "✅ Laser & Buzzer turned ON");
        }
        else if (text == "/all off") {
          digitalWrite(LASER_PIN, LOW);
          digitalWrite(BUZZER_PIN, LOW); // Turn both off
          bot.sendMessage(chat_id, "❎ Laser & Buzzer turned OFF");
        }
        else if (text == "/status") {
          String status = "*Device Status:*\n";
          status += "🔆 Laser: " + String(digitalRead(LASER_PIN) ? "ON" : "OFF") + "\n";
          status += "🔊 Buzzer: " + String(digitalRead(BUZZER_PIN) ? "ON" : "OFF");
          bot.sendMessage(chat_id, status, "Markdown");
        } 
        else {
          bot.sendMessage(chat_id, "❓ Available commands:\n"
                                   "`/laser on`, `/laser off`\n"
                                   "`/buzzer on`, `/buzzer off`\n"
                                   "`/all on`, `/all off`\n"
                                   "`/status`", "Markdown");
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTelegramCheck = millis();  // Update the last check time
  }
}


#endif // NOTIFICATION_MANAGER_H
