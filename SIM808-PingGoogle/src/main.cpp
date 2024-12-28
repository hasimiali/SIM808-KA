#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobot_SIM808.h>
DFRobot_SIM808 sim808(&mySerial); // Inisialisasi modul SIM808
SoftwareSerial mySerial(8, 7);    // RX, TX pins untuk SIM808

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  while (!sim808.init())
  {
    delay(2000);
    Serial.println("SIM808 init error");
  }
  Serial.println("SIM808 init success");

  Serial.println("Connecting to GPRS...");
  if (connectToGPRS())
  {
    Serial.println("GPRS connected.");
  }
  else
  {
    Serial.println("GPRS connection failed.");
  }

  pingGoogle();
}

void loop()
{
}

bool connectToGPRS()
{
  // Attach to GPRS network
  mySerial.println("AT+CGATT=1"); // Attach to GPRS
  delay(2000);

  mySerial.println("AT+CSTT=\"internet\"");
  delay(2000);

  mySerial.println("AT+CIICR");
  delay(8000);

  mySerial.println("AT+CIFSR");
  delay(5000);

  while (mySerial.available() > 0)
  {
    Serial.write(mySerial.read());
  }

  return true;
}

void pingGoogle()
{
  Serial.println("Pinging Google...");
  mySerial.println("AT+CIPPING=\"www.google.com\""); // Ping Google
  delay(5000);

  while (mySerial.available() > 0)
  {
    Serial.write(mySerial.read());
  }
}
