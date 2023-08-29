#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Robin";
const char* password = "12345670";

// Replace with your ThingSpeak API key and channel ID
const char* apiKey = "ZSRVNBXWEX74IUP1";
const long channelId = 2128315;

// Replace with your DHT sensor pin
const int DHTPIN = 19;
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("Connected to Wi-Fi");

  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read DHT sensor data");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  // Send the sensor data to ThingSpeak
  WiFiClient client;
  HTTPClient http;

  String url = "http://api.thingspeak.com/update?api_key=";
  url += apiKey;
  url += "&field1=";
  url += String(temp);
  url += "&field2=";
  url += String(hum);

  Serial.print("Sending data to ThingSpeak: ");
  Serial.println(url);

  http.begin(client, url);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }

  http.end();

  delay(5000); // Wait for 5 seconds before sending the next data
}