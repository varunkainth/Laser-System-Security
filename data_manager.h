#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "config.h"

// Updates the NTP client and returns current time as a formatted string (HH:MM:SS)
String getTimeString() {
  if (!timeClient.update()) {
    Serial.println("⚠️ Failed to update time from NTP");
  }
  return timeClient.getFormattedTime();
}

// Returns current Unix timestamp (epoch time)
long getCurrentTimestamp() {
  int retries = 0;
  while (retries < NTP_RETRY_COUNT) {
    if (timeClient.update()) {
      return timeClient.getEpochTime();
    }
    retries++;
    Serial.println("⚠️ Failed to update time from NTP. Retrying...");
    delay(NTP_RETRY_DELAY);
  }
  Serial.println("❌ NTP update failed after multiple retries.");
  return 0;  // Return a default value (0) if NTP update fails
}

// Returns formatted full date and time: YYYY-MM-DD HH:MM:SS
String getFullDateTimeString() {
  long rawTime = getCurrentTimestamp();
  
  if (rawTime == 0) {
    return "0000-00-00 00:00:00";  // Return default value if NTP update fails
  }

  struct tm* ti = localtime(&rawTime);
  
  // Using char array to avoid String object allocation
  char buffer[30];
  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d", 
           ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday,
           ti->tm_hour, ti->tm_min, ti->tm_sec);
  return String(buffer);
}

#endif // DATA_MANAGER_H
