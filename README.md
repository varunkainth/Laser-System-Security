# Laser System Security

## Overview
This project is a security system based on a laser detection mechanism using an ESP8266 microcontroller. The system monitors a laser beam and triggers various actions when the beam is interrupted. These actions include sound alerts via a buzzer, sending notifications through Discord and email using webhooks, and storing event data in Firebase. The system includes user authentication and supports multiple user emails for managing alerts.

## Features
- **Laser Detection**: Monitors if the laser beam is blocked and triggers an alert.
- **Buzzer**: A buzzer sounds when the laser beam is blocked.
- **Notifications**: Sends alerts through Discord and email using webhooks.
- **Event Logging**: Stores events in Firebase for tracking and later analysis.
- **User Authentication**: Supports multiple users for managing alerts and notifications.
- **Web Interface**: Web server for controlling the laser, viewing events, and managing user settings.

## Components
- **ESP8266**: Microcontroller used to manage the laser detection system.
- **LDR (Light Dependent Resistor)**: Detects laser beam blockage.
- **Laser**: Can be toggled on or off via the web server.
- **Buzzer**: Provides audible alerts when the laser is blocked.
- **Firebase**: Stores event data for later access.
- **Webhooks**: Used for sending notifications via Discord and email.

## Libraries
- `ESP8266WiFi`: For connecting the ESP8266 to WiFi.
- `Firebase ESP8266`: For interacting with Firebase.
- `ESP8266WebServer`: For setting up the web server.
- `ArduinoJson`: For handling JSON data in web responses.
- `DiscordWebhook`: For sending alerts to Discord.

## Setup
### Hardware
1. **ESP8266**: Any ESP8266 board (e.g., NodeMCU, Wemos D1 Mini).
2. **LDR**: Connect the LDR in a voltage divider configuration with a resistor.
3. **Laser**: A 5V laser module.
4. **Buzzer**: Connect the buzzer to a GPIO pin of the ESP8266.
5. **Optional**: You can also add external sensors or components like a camera for extra features.

### Software
1. Clone the repository:
   ```bash
   git clone https://github.com/varunkainth/Laser-System-Security.git
   cd Laser-System-Security
