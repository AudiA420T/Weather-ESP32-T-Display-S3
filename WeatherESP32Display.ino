#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
//Temp sensor stuff
#include <DHT.h>
#define DHT11PIN 13
DHT dht(DHT11PIN, DHT11);

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

// WiFi Credentials
const char* ssid = "YourNetworkName";
const char* password = "YourPassword";

// API Endpoint
String URL = "https://api.tomorrow.io/v4/weather/forecast?location=LAT,LONG&apikey=APIKEY";

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD
  tft.init();
  tft.setRotation(3);

  /* Start the DHT11 Sensor */
  dht.begin();

  // Fill the screen with a couple of colors on bootup
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.fillScreen(TFT_BLACK);

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    yield(); // Reset watchdog
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(URL);

    int httpCode = http.GET();
    yield(); // Reset watchdog

    float humi = dht.readHumidity();
    //true is used for F over C
    float temp = dht.readTemperature(true);
    float tempC = dht.readTemperature();

    if (httpCode > 0) {
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);
      yield(); // Reset watchdog

      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc["timelines"]["minutely"][0]["values"];

      // Extract temperature, wind speed, and UV index
      float tempCelsius = obj["temperature"];
      int tempFahrenheit = (int)((tempCelsius * 9.0 / 5.0) + 32.0); // Convert to Fahrenheit and truncate to int

      // Extract wind speed
      float windSpeedFloat = obj["windSpeed"];
      String windSpeedStr = String(windSpeedFloat, 1); // Convert to string with one decimal place

      int uvIndex = obj["uvIndex"];

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

    } else {
      Serial.println("Error in HTTP request");
    }

    http.end();
  }

  // Wait for 10 min
  for (int i = 0; i < 600; i++) {
    delay(1000); // Delay in smaller increments to call yield() frequently
    yield(); // Reset watchdog
  }
}