#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include "config.h"
#include "sensors.h"
#include "firebase_manager.h"

// Forward declarations of handler functions
void handleRoot();
void handleLogin();
void handleDashboard();
void handleControl();
void handleData();
void handleToggleSystem();
void handleToggleLaser();
void handleToggleBuzzer();
void handleClearData();
void handleNotFound();

// Forward declarations of page generator functions
String getLoginPage();
String getDashboardPage();
String getControlPage();
String getDataPage();

void setupWebServer() {
  // Configure web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/dashboard", HTTP_GET, handleDashboard);
  server.on("/control", HTTP_GET, handleControl);
  server.on("/data", HTTP_GET, handleData);
  server.on("/toggle-system", HTTP_POST, handleToggleSystem);
  server.on("/toggle-laser", HTTP_POST, handleToggleLaser);
  server.on("/toggle-buzzer", HTTP_POST, handleToggleBuzzer);
  server.on("/clear-data", HTTP_GET, handleClearData);
  server.onNotFound(handleNotFound);
  
  // Start web server
  server.begin();
  Serial.println("HTTP server started");
}

// Web server route handlers
void handleRoot() {
  String html = getLoginPage();
  server.send(200, "text/html", html);
}

void handleLogin() {
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == webUsername && server.arg("password") == webPassword) {
      server.sendHeader("Location", "/dashboard");
      server.send(303);
    } else {
      server.send(401, "text/html", "<html><body><p>Invalid credentials</p><a href='/'>Go back</a></body></html>");
    }
  } else {
    server.send(400, "text/html", "<html><body><p>Bad request</p><a href='/'>Go back</a></body></html>");
  }
}

void handleDashboard() {
  String html = getDashboardPage();
  server.send(200, "text/html", html);
}

void handleControl() {
  String html = getControlPage();
  server.send(200, "text/html", html);
}

void handleData() {
  String html = getDataPage();
  server.send(200, "text/html", html);
}

void handleToggleSystem() {
  systemArmed = !systemArmed;
  updateSystemStatus();  // Ensure this function is defined elsewhere
  server.sendHeader("Location", "/control");
  server.send(303);
}

void handleToggleLaser() {
  setLaserState(!digitalRead(LASER_PIN));  // Ensure these functions are defined
  server.sendHeader("Location", "/control");
  server.send(303);
}

void handleToggleBuzzer() {
  setBuzzerState(!digitalRead(BUZZER_PIN));  // Ensure these functions are defined
  server.sendHeader("Location", "/control");
  server.send(303);
}

void handleClearData() {
  if (clearAllAlarms()) {  // Ensure clearAllAlarms is defined and works
    server.send(200, "text/html", "<html><body><p>All data cleared successfully.</p><a href='/data'>Back to Data</a></body></html>");
  } else {
    server.send(500, "text/html", "<html><body><p>Failed to clear data.</p><a href='/data'>Back to Data</a></body></html>");
  }
}

void handleNotFound() {
  server.send(404, "text/html", "<html><body><h1>404 Not Found</h1><a href='/'>Go home</a></body></html>");
}

// HTML page generation functions
String getLoginPage() {
  String html = "<html><head><title>Laser Security System</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: 'Arial', sans-serif; background-color: #f4f4f9; margin: 0; padding: 20px; text-align: center; }";
  html += "form { max-width: 400px; margin: 0 auto; padding: 30px; background-color: #fff; box-shadow: 0 0 15px rgba(0, 0, 0, 0.1); border-radius: 10px; }";
  html += "input { width: 100%; padding: 12px; margin: 10px 0; font-size: 14px; border: 1px solid #ccc; border-radius: 5px; }";
  html += "input[type='submit'] { background-color: #4CAF50; color: white; border: none; cursor: pointer; padding: 15px; font-size: 16px; border-radius: 5px; }";
  html += "input[type='submit']:hover { background-color: #45a049; }";
  html += "</style></head><body>";
  html += "<h1 style='font-size: 2.5em; color: #333;'>Laser Security System</h1>";
  html += "<p style='font-size: 1.2em;'>Current time: " + getFormattedDateTime() + "</p>";
  html += "<form action='/login' method='POST'>";
  html += "<h2>Login</h2>";
  html += "Username: <input type='text' name='username' required><br>";
  html += "Password: <input type='password' name='password' required><br>";
  html += "<input type='submit' value='Login'>";
  html += "</form></body></html>";
  return html;
}

String getDashboardPage() {
  String html = "<html><head><title>Dashboard</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: 'Arial', sans-serif; margin: 0; padding: 20px; text-align: center; background-color: #f4f4f9; }";
  html += ".btn { display: inline-block; padding: 12px 24px; margin: 15px; background-color: #4CAF50; color: white; text-decoration: none; border-radius: 8px; font-size: 16px; }";
  html += ".btn-danger { background-color: #f44336; }";
  html += ".status { padding: 15px; margin: 20px 0; border-radius: 8px; font-size: 18px; }";
  html += ".status-on { background-color: #dff0d8; color: #3c763d; border: 1px solid #d6e9c6; }";
  html += ".status-off { background-color: #f2dede; color: #a94442; border: 1px solid #ebccd1; }";
  html += "</style></head><body>";
  html += "<h1 style='font-size: 2.5em; color: #333;'>Laser Security System Dashboard</h1>";
  html += "<p style='font-size: 1.2em;'>Welcome, " + String(webUsername) + " | " + getFormattedDateTime() + "</p>";
  
  html += "<div class='status " + String(systemArmed ? "status-on" : "status-off") + "'>";
  html += "System Status: " + String(systemArmed ? "ARMED" : "DISARMED");
  html += "</div>";
  
  html += "<h2>Actions</h2>";
  html += "<a href='/control' class='btn'>System Control</a> ";
  html += "<a href='/data' class='btn'>View Data</a>";
  
  html += "</body></html>";
  return html;
}

String getControlPage() {
  String html = "<html><head><title>System Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: 'Arial', sans-serif; margin: 0; padding: 20px; text-align: center; background-color: #f4f4f9; }";
  html += ".btn { display: inline-block; padding: 12px 24px; margin: 15px; background-color: #4CAF50; color: white; text-decoration: none; border-radius: 8px; font-size: 16px; }";
  html += ".btn-danger { background-color: #f44336; }";
  html += ".status { padding: 15px; margin: 20px 0; border-radius: 8px; font-size: 18px; }";
  html += ".status-on { background-color: #dff0d8; color: #3c763d; border: 1px solid #d6e9c6; }";
  html += ".status-off { background-color: #f2dede; color: #a94442; border: 1px solid #ebccd1; }";
  html += "</style></head><body>";
  html += "<h1 style='font-size: 2.5em; color: #333;'>System Control</h1>";
  html += "<p style='font-size: 1.2em;'>User: " + String(webUsername) + " | " + getFormattedDateTime() + "</p>";
  
  html += "<div class='status " + String(systemArmed ? "status-on" : "status-off") + "'>";
  html += "System Status: " + String(systemArmed ? "ARMED" : "DISARMED");
  html += "</div>";
  
  html += "<h2>Controls</h2>";
  html += "<form action='/toggle-system' method='POST'>";
  html += "<button type='submit' class='btn " + String(systemArmed ? "btn-danger" : "") + "'>";
  html += String(systemArmed ? "Disarm System" : "Arm System");
  html += "</button></form>";
  
  html += "<form action='/toggle-laser' method='POST'>";
  html += "<button type='submit' class='btn'>";
  html += String(digitalRead(LASER_PIN) ? "Turn Off Laser" : "Turn On Laser");
  html += "</button></form>";
  
  html += "<form action='/toggle-buzzer' method='POST'>";
  html += "<button type='submit' class='btn'>";
  html += String(digitalRead(BUZZER_PIN) ? "Turn Off Buzzer" : "Turn On Buzzer");
  html += "</button></form>";

  html += "</body></html>";
  return html;
}

String getDataPage() {
  String html = "<html><head><title>System Data</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: 'Arial', sans-serif; margin: 0; padding: 20px; text-align: center; background-color: #f4f4f9; }";
  html += ".btn { display: inline-block; padding: 12px 24px; margin: 15px; background-color: #4CAF50; color: white; text-decoration: none; border-radius: 8px; font-size: 16px; }";
  html += ".btn-danger { background-color: #f44336; }";
  html += "</style></head><body>";
  html += "<h1 style='font-size: 2.5em; color: #333;'>System Data</h1>";
  html += "<p style='font-size: 1.2em;'>Current data of laser security system</p>";
  
  html += "<h2>Events:</h2>";
  html += "<ul>";
  // Display event data here
  html += "</ul>";

  html += "<a href='/control' class='btn'>Back to Control</a>";
  html += "<a href='/clear-data' class='btn btn-danger'>Clear All Data</a>";
  
  html += "</body></html>";
  return html;
}

#endif // WEB_INTERFACE_H
