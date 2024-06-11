#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT test!");
  dht.begin();
}

void loop() {
  float h,t,f;
  
  h = dht.readHumidity(); // Humidity in %
  t = dht.readTemperature(); // Temperature in Celsius
  f = dht.readTemperature(true); // Temperature in Fahrenheit
  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C "); 
  Serial.print(f);
  Serial.println(" °F ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.println(" °C ");
  Serial.print(hif);
  Serial.println(" *F");
}
