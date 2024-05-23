# Mountain Climbing and Health Monitoring System

This project involves an ESP8266 microcontroller interfaced with multiple sensors to gather and transmit environmental data, GPS location, and heart rate information. The data is displayed on an LCD and sent to a specified Telegram chat.

## Features

- **Environmental Monitoring**: Measures temperature and humidity using a DHT11 sensor.
- **GPS Tracking**: Retrieves GPS coordinates using the NEO-6M GPS module.
- **Heart Rate Monitoring**: Reads heart rate data via a pulse sensor.
- **Temperature Monitoring**: Uses a DS18B20 sensor to measure temperature.
- **Data Display**: Shows data on an I2C LCD.
- **Data Transmission**: Sends the collected data to a Telegram bot.

## Hardware Required

- ESP8266 (e.g., NodeMCU)
- NEO-6M GPS module
- DHT11 temperature and humidity sensor
- DS18B20 temperature sensor
- Pulse sensor
- I2C LCD
- Push button
- Connecting wires
- Breadboard

## Pin Configuration

| Component       | ESP8266 Pin     |
| --------------- | --------------- |
| Button          | D5              |
| GPS Tx          | D6              |
| GPS Rx          | D7              |
| DHT11           | D3              |
| Pulse Sensor    | A0              |
| LCD SDA         | D2              |
| LCD SCL         | D1              |

## Libraries Required

- `SoftwareSerial`
- `TinyGPS++`
- `ESP8266WiFi`
- `DHT`
- `OneWire`
- `DallasTemperature`
- `LiquidCrystal_I2C`

## Connections

- **NEO-6M GPS Module**: Connect Tx to D6 and Rx to D7.
- **DHT11 Sensor**: Connect data pin to D3.
- **Pulse Sensor**: Connect analog output to A0.
- **DS18B20 Sensor**: Connect data pin to A0 via OneWire.
- **LCD Display**: Connect SDA to D2 and SCL to D1.

## Setup

### Install Arduino Libraries

Ensure the following libraries are installed in your Arduino IDE:

- `SoftwareSerial`
- `TinyGPS++`
- `ESP8266WiFi`
- `DHT`
- `OneWire`
- `DallasTemperature`
- `LiquidCrystal_I2C`

### Configure WiFi and Telegram Bot

1. Replace `"your_telegram_bot_token"` with your Telegram bot token.
2. Replace `"your_telegram_chat_id"` with your Telegram chat ID.
3. Update `const char *ssid` and `const char *password` with your WiFi credentials.
