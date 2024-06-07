#define BLYNK_TEMPLATE_ID "BLYNKTEMPLATEID"
#define BLYNK_TEMPLATE_NAME "BLYNKTEMPLATENAME"
#define BLYNK_AUTH_TOKEN "BLYNKAUTHTOKEN"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// include the images
#include "cloudy.h"
#include "foggy.h"
#include "overcast.h"
#include "rainy.h"
#include "sunny.h"
#include "rainbow.h"
#include "moon.h"

#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
// Temp sensor stuff
#include <DHT.h>
#define DHT11PIN 13
DHT dht(DHT11PIN, DHT11);

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

// WiFi Credentials
const char* ssid = "YourNetworkName";
const char* password = "YourNetworkPassword";

// URL Endpoint for the API
String baseURL = "https://api.tomorrow.io/v4/weather/forecast?location=";
String apiKey = "APIKEY";

// Replace with your location credentials
String lat = "LAT";
String lon = "LONG";

// API Endpoint
String URL = baseURL + lat + "," + lon + "&apikey=" + apiKey;

BlynkTimer uptimeTimer;
BlynkTimer weatherApiTimer;
BlynkTimer dhtSensorTimer;

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  // Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  // Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  // Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void uptimeEvent() {
  // You can send any value at any time.
  // Please don't send more than 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  yield(); // Reset watchdog
}

// This function shows the right image depending on the type of weather
void displayWeatherIcon(int weatherCode) {

  tft.fillRect(200, 43, 64, 64, TFT_BLACK); // Clear previous icon

  switch (weatherCode) {
    case 1000:
      tft.pushImage(200, 43, 64, 64, sunny);
      break;
    case 1100:
      tft.pushImage(200, 43, 64, 64, sunny);
      break;
    case 1101:
      tft.pushImage(200, 43, 64, 64, overcast);
      break;
    case 1102:
    tft.pushImage(200, 43, 64, 64, overcast);
    break;
    case 1001:
    tft.pushImage(200, 43, 64, 64, overcast);
    break;
    case 2000:
    tft.pushImage(200, 43, 64, 64, overcast);
    break;
    case 2100:
    tft.pushImage(200, 43, 64, 64, overcast);
    break;
    case 4000:
    tft.pushImage(200, 43, 64, 64, rainy);
    break;
    case 4001:
    tft.pushImage(200, 43, 64, 64, rainy);
    break;
    case 4200:
    tft.pushImage(200, 43, 64, 64, rainy);
    break;
    case 4201:
    tft.pushImage(200, 43, 64, 64, rainy);
    break;
    default:
      tft.pushImage(200, 43, 64, 64, rainbow);
      break;
  }
}

// This function grabs weather data from the API
void fetchWeatherData() {
   // Wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(URL);

    int httpCode = http.GET();
    yield(); // Reset watchdog

    if (httpCode > 0) {
      String JSON_Data = http.getString();
      yield(); // Reset watchdog

      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc["timelines"]["minutely"][0]["values"];

      // Loop through each key-value pair in the obj and print them
      for (JsonPair kv : obj) {
        Serial.print(kv.key().c_str());
        Serial.print(": ");
        Serial.println(kv.value().as<String>());
      }

      // Extract temperature, wind speed, and UV index
      float tempCelsius = obj["temperature"];
      int tempFahrenheit = (int)((tempCelsius * 9.0 / 5.0) + 32.0); // Convert to Fahrenheit and truncate to int

      // Extract wind speed
      float windSpeedFloat = obj["windSpeed"];
      String windSpeedStr = String(windSpeedFloat, 1); // Convert to string with one decimal place

      // Extract weather
      int weatherCode = obj["weatherCode"];

      int uvIndex = obj["uvIndex"];

      // Read internal sensor data
      float humi = dht.readHumidity();
      float temp = dht.readTemperature(true);

      // Send data to Blynk
      Blynk.virtualWrite(V3, temp);  // V3 is for internal temperature
      Blynk.virtualWrite(V4, humi);  // V4 is for humidity

      // Display the data on the TFT screen
      tft.fillScreen(TFT_BLACK); // Set background to black
      tft.setCursor(0, 0, 2);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setTextSize(2.5);
      tft.println("Ext Temp: " + String(tempFahrenheit) + "F");
      tft.println("Int Temp: " + String(int(round(temp))) + "F");
      tft.println("Int Hum: " + String(int(humi)) + "%");
      tft.println("Wind: " + windSpeedStr + " m/s");
      tft.println("UV Index: " + String(uvIndex));
      //showing image
      displayWeatherIcon(weatherCode);

    } else {
      Serial.println("Error in HTTP request");
    }

    http.end();
  }
}

// This function grabs the DHT data and updates the specific LCD lines
void updateDhtData() {
  // Read internal sensor data
  float humi = dht.readHumidity();
  float temp = dht.readTemperature(true);
  
  // Send data to Blynk
  Blynk.virtualWrite(V3, temp);  // V3 is for internal temperature
  Blynk.virtualWrite(V4, humi);  // V4 is for humidity

  // Update internal temperature
  tft.setCursor(0, 32, 2.5);
  tft.setTextSize(2.5);
  tft.println("Int Temp: " + String(int(round(temp))) + "F");
  // Update internal humidity
  tft.setCursor(0, 66, 2.5);
  tft.setTextSize(2.5);
  tft.println("Int Hum: " + String(int(humi)) + "%");
}

void setup() {
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Setup a function to be called every second
  uptimeTimer.setInterval(1000L, uptimeEvent);
  weatherApiTimer.setInterval(600000L, fetchWeatherData); // Every 10 minutes
  dhtSensorTimer.setInterval(10000L, updateDhtData); // Every 10 seconds
  
  // Initialize LCD
  tft.init();
  tft.setRotation(3);
  //for image
  tft.setSwapBytes(true);

  // Start the DHT11 Sensor
  dht.begin();

  // Fill the screen with a couple of colors on bootup
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.fillScreen(TFT_BLACK);

  fetchWeatherData();
}

void loop() {
  Blynk.run();
  uptimeTimer.run();
  weatherApiTimer.run();
  dhtSensorTimer.run();
}