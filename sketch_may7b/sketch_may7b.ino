#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "esp32";
const char* password = "12345678";

String apiKey = "0e16e4a23e89cd9c6cba1c1c820e30b9 ";

float latitude;
float longitude;

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  getLocation();

  delay(2000);

  getWeather();
}

void loop() {

}

void getLocation() {

  HTTPClient http;

  // FREE IP LOCATION API
  String url = "http://ip-api.com/json/";

  http.begin(url);

  int httpCode = http.GET();

  if (httpCode > 0) {

    String payload = http.getString();

    Serial.println(payload);

    DynamicJsonDocument doc(2048);

    deserializeJson(doc, payload);

    latitude = doc["lat"];
    longitude = doc["lon"];

    String city = doc["city"];
    String region = doc["regionName"];
    String country = doc["country"];

    Serial.println("========== LOCATION ==========");

    Serial.print("City: ");
    Serial.println(city);

    Serial.print("Region: ");
    Serial.println(region);

    Serial.print("Country: ");
    Serial.println(country);

    Serial.print("Latitude: ");
    Serial.println(latitude);

    Serial.print("Longitude: ");
    Serial.println(longitude);

  } else {

    Serial.println("Location Error");
  }

  http.end();
}

void getWeather() {

  HTTPClient http;

  String url =
    "https://api.openweathermap.org/data/2.5/weather?lat=" +
    String(latitude) +
    "&lon=" +
    String(longitude) +
    "&units=metric&appid=" +
    apiKey;

  Serial.println(url);

  http.begin(url);

  int httpCode = http.GET();

  if (httpCode > 0) {

    String payload = http.getString();

    Serial.println(payload);

    DynamicJsonDocument doc(4096);

    deserializeJson(doc, payload);

    String weather =
      doc["weather"][0]["main"].as<String>();

    float temp =
      doc["main"]["temp"];

    String place =
      doc["name"].as<String>();

    Serial.println("========== WEATHER ==========");

    Serial.print("Place: ");
    Serial.println(place);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    Serial.print("Weather: ");
    Serial.println(weather);

  } else {

    Serial.println("Weather API Error");
  }

  http.end();
}