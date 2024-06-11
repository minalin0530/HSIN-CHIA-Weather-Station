#include <TinyGPS++.h>
#include <SoftwareSerial.h>

int RXPin = 8;
int TXPin = 9;
int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
}

void loop() {
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  if (millis() > 10000 && gps.charsProcessed() < 20)
  {
    BTSerial.println("No GPS detected");
  }
}

void displayInfo()
{ 
  if (gps.location.isValid())
  {
    BTSerial.print("Latitude: ");
    BTSerial.println(gps.location.lat(), 6);
    BTSerial.print("Longitude: ");
    BTSerial.println(gps.location.lng(), 6);
    BTSerial.print("Altitude: ");
    BTSerial.println(gps.altitude.meters());
  }
  else
  {
    BTSerial.println("Location: Not Available");
  }
  
  BTSerial.print("Date: ");
  if (gps.date.isValid())
  {
    BTSerial.print(gps.date.month());
    BTSerial.print("/");
    BTSerial.print(gps.date.day());
    BTSerial.print("/");
    BTSerial.println(gps.date.year());
  }
  else
  {
    BTSerial.println("Not Available");
  }

  BTSerial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour()+8 < 10) BTSerial.print(F("0"));
    BTSerial.print(gps.time.hour()+8);
    BTSerial.print(":");
    if (gps.time.minute() < 10) BTSerial.print(F("0"));
    BTSerial.print(gps.time.minute());
    BTSerial.print(":");
    if (gps.time.second() < 10) BTSerial.print(F("0"));
    BTSerial.print(gps.time.second());
    BTSerial.print(".");
    if (gps.time.centisecond() < 10) BTSerial.print(F("0"));
    BTSerial.println(gps.time.centisecond());
  }
  else
  {
    BTSerial.println("Not Available");
  }

  BTSerial.println();
  BTSerial.println();
  delay(1000);
}
