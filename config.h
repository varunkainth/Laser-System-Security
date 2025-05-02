/*
 * Configuration file containing all settings and global variables
 */

#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions
#define LASER_PIN 5     // D1
#define BUZZER_PIN 4    // D2
#define LDR_PIN A0      // Analog input A0


#define DEBUG

#define DEBUG_HEAP true
// WiFi credentials
// const char* ssid = "Let's Rock";
// const char* password = "09876543210";

const char* ssid = "PPC18309-2.4G";
const char* password = "Dpk@882658@@";               

// Web server credentials
const char* webUsername = "admin";
const char* webPassword = "admin";

// Blynk credentials
// #define BLYNK_TEMPLATE_ID "TMPL3LrXwuhZi"
// #define BLYNK_TEMPLATE_NAME "Laser Block"
// #define BLYNK_AUTH_TOKEN "XwpheVqhwSvkc4wNYG_FOX1BhammBkGE"
// #define BLYNK_PRINT Serial  // Set serial output for debugging
// // User information
// #define USER_NAME "varunkainth"

// Telegram bot credentials
#define BOT_TOKEN "7665868349:AAGZNaGxxeGX2Tkl5_tNjBbUtFPQvAQjy2Q"
#define CHAT_ID "6481362471"

// Firebase credentials
#define FIREBASE_HOST "laserproject-fac89-default-rtdb.asia-southeast1.firebasedatabase.app"
#define DATABASE_SECRET "wXmLCfckvGXCbwY7b9PwTXOgV6DII4uyNB23aOUm"

// System variables
extern bool systemArmed;
extern bool alarmActive;
extern unsigned long lastAlertTime;

// Constant Definitions
const int ldrThreshold = 500; // Adjust based on your environment
const unsigned long alertCooldown = 30000; // 30 seconds cooldown between alerts
const unsigned long telegramCheckInterval = 2000; // 2 seconds for Telegram checks
#define NTP_RETRY_COUNT 3
#define NTP_RETRY_DELAY 5000

// Global objects
extern ESP8266WebServer server;
extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;
extern FirebaseData firebaseData;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

#endif // CONFIG_H