#include <Arduino.h>
#include <DFRobot_SIM808.h>
SoftwareSerial mySerial(8, 7); // sim808 serial
DFRobot_SIM808 sim808(&mySerial);
float lat, lon, speed;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  // Initialize sim808 module
  while (!sim808.init())
  {
    delay(2000);
    Serial.println("Sim808 init error");
  }
  Serial.println("Sim808 init success");

  // Turn on the GPS power
  if (sim808.attachGPS())
  {
    Serial.println("Open the GPS power success");
  }
  else
  {
    Serial.println("Open the GPS power failure");
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
  }
  else
  {
    Serial.println("GPS not fixed");
  }
}

void loop()
{
  // Call the function to get and display GPS data
  mySerial.println("AT+CGNSPWR=1"); // Aktifkan GPS
  delay(1000);

  getGPSData();
  delay(1000); // Delay for 1 second
}