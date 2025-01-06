#include <Arduino.h>
#include <DFRobot_SIM808.h>
#include <SoftwareSerial.h>

#define PIN_TX 8
#define PIN_RX 7
SoftwareSerial mySerial(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&mySerial);

// HTTP buffer
char buffer[512];

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);

  // Initialize SIM808
  while (!sim808.init())
  {
    delay(1000);
    Serial.println("SIM808 init error");
  }
  delay(3000);

  // Join the network
  while (!sim808.join(F("cmnet")))
  {
    Serial.println("SIM808 join network error");
    delay(2000);
  }
  Serial.print("IP Address: ");
  Serial.println(sim808.getIPAddress());

  // Establish a TCP connection to httpbin.org
  if (!sim808.connect(TCP, "httpbin.org", 80))
  {
    Serial.println("Connect error");
    return;
  }
  Serial.println("Connected to httpbin.org");

  // Construct the GET request
  String httpCmd = "GET /get HTTP/1.1\r\n";
  httpCmd += "Host: httpbin.org\r\n";
  httpCmd += "Connection: close\r\n";
  httpCmd += "\r\n";

  // Send the HTTP GET request
  sim808.send(httpCmd.c_str(), httpCmd.length());

  // Receive the response
  Serial.println("Receiving response...");
  while (true)
  {
    int ret = sim808.recv(buffer, sizeof(buffer) - 1);
    if (ret <= 0)
    {
      Serial.println("Request complete");
      break;
    }
    buffer[ret] = '\0';
    Serial.print("Response: ");
    Serial.println(buffer);
  }

  // Close the connection
  sim808.close();

  // Disconnect from the network
  sim808.disconnect();
}

void loop()
{
  // Nothing to do in the loop
}
