#include <Arduino.h>

#include <Arduino.h>
#include <DFRobot_SIM808.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

SoftwareSerial mySerial(8, 7); // sim808 serial
DFRobot_SIM808 sim808(&mySerial);

const char *ssid = "koding akademi";
const char *password = "12345678";
const char *firestore_url = "https://firestore.googleapis.com/v1/projects/YOUR_PROJECT_ID/databases/(default)/documents/gps_data";

float lat, lon, speed;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize SIM808
  while (!sim808.init())
  {
    delay(2000);
    Serial.println("Sim808 init error");
  }
  Serial.println("Sim808 init success");

  // Turn on GPS
  if (sim808.attachGPS())
  {
    Serial.println("Open the GPS power success");
  }
  else
  {
    Serial.println("Open the GPS power failure");
  }
}

void sendToFirestore(float lat, float lon, float speed)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(firestore_url);
    http.addHeader("Content-Type", "application/json");

    // Prepare JSON payload
    StaticJsonDocument<200> doc;
    doc["fields"]["latitude"]["doubleValue"] = lat;
    doc["fields"]["longitude"]["doubleValue"] = lon;
    doc["fields"]["speed"]["doubleValue"] = speed;

    String payload;
    serializeJson(doc, payload);

    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0)
    {
      Serial.print("Data sent successfully. Response code: ");
      Serial.println(httpResponseCode);
    }
    else
    {
      Serial.print("Error sending data. HTTP response code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}

void getGPSData()
{
  if (sim808.getGPS())
  {
    lat = sim808.GPSdata.lat;
    lon = sim808.GPSdata.lon;
    speed = sim808.GPSdata.speed_kph;

    Serial.print("Latitude: ");
    Serial.println(lat, 5);
    Serial.print("Longitude: ");
    Serial.println(lon, 5);
    Serial.print("Speed (kph): ");
    Serial.println(speed, 5);

    // Send data to Firestore
    sendToFirestore(lat, lon, speed);
  }
  else
  {
    Serial.println("GPS not fixed");
  }
}

void loop()
{
  // Activate GPS
  mySerial.println("AT+CGNSPWR=1");
  delay(1000);

  getGPSData();
  delay(5000); // Send data every 5 seconds
}
