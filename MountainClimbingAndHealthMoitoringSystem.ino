#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_PIN D5
#define gpsTxPin D6
#define gpsRxPin D7
#define DHT_PIN D3
#define DHT_TYPE DHT11
#define LCD_SDA D2
#define LCD_SCL D1
#define PULSE_SENSOR A0

SoftwareSerial neo6m(gpsTxPin, gpsRxPin);
DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(PULSE_SENSOR);
DallasTemperature sensors(&oneWire);
TinyGPSPlus gps;

const char *ssid = "Ashutosh";
const char *password = "qwermnbv";

const int I2C_ADDR = 0x27;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

String TELEGRAM_BOT_TOKEN = "your_telegram_bot_token";
String TELEGRAM_CHAT_ID = "your_telegram_chat_id";

void setup() {
  Serial.begin(115200);
  neo6m.begin(9600);
  dht.begin();
  sensors.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.begin(LCD_COLS, LCD_ROWS);
  Wire.begin(LCD_SDA, LCD_SCL);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperatureDHT = dht.readTemperature();
  float humidity = dht.readHumidity();
  float temperatureDS18B20 = getDS18B20Temperature();
  int heartRate = getHeartRate();

  smartdelay_gps(1000);

  if (!isnan(temperatureDHT)) {
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6);

    String message = "GPS Location: http://maps.google.com/maps?q=" + latitude + "," + longitude +
                     "\nEnv. Temperature: " + String(temperatureDHT, 1) + " °C" +
                     "\nHumidity: " + String(humidity, 1) + " %" +
                     "\nDS18B20 Temp.: " + String(temperatureDS18B20, 1) + " °C" +
                     "\nHeart Rate: " + String(heartRate) + " bpm";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Env. Temp: " + String(temperatureDHT, 1) + " C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: " + String(humidity, 1) + " %");

    sendTelegramMessage(message);

    Serial.println("Latitude: " + latitude);
    Serial.println("Longitude: " + longitude);
    Serial.println("Env. Temperature: " + String(temperatureDHT, 1) + " °C");
    Serial.println("Humidity: " + String(humidity, 1) + " %");
    Serial.println("DS18B20 Temp.: " + String(temperatureDS18B20, 1) + " °C");
    Serial.println("Heart Rate: " + String(heartRate) + " bpm");
    Serial.println();
  }

  delay(20000); // 20 seconds
}

static void smartdelay_gps(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (neo6m.available())
      gps.encode(neo6m.read());
  } while (millis() - start < ms);
}

float getDS18B20Temperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

int getHeartRate() {
  int heartRate = analogRead(PULSE_SENSOR);
  return heartRate;
}

void sendTelegramMessage(String message) {
  WiFiClientSecure client;
  client.setInsecure();

  String url = "/bot" + TELEGRAM_BOT_TOKEN + "/sendMessage";
  String postData = "chat_id=" + TELEGRAM_CHAT_ID + "&text=" + message;

  if (client.connect("api.telegram.org", 443)) {
    Serial.println("Connected to Telegram server");
    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: api.telegram.org");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);

    delay(500); 

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    client.stop();
    Serial.println("Telegram message sent successfully");
  } else {
    Serial.println("Failed to connect to Telegram server");
  }
}
