// light begin
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;
// light end

//溫溼度begin
#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
// 溫溼度end

// bluetooth start
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // Pin 10 -->  TX, Pin 11 -->  RX 
char bluetoothData;
// bluetooth end

// GPS begin
#include <TinyGPS++.h>

#include <AltSoftSerial.h>
int RXPin = 8;
int TXPin = 9;
int GPSBaud = 9600;
TinyGPSPlus gps;
AltSoftSerial gpsSerial;
// GPS end

void setup() {
  Wire.begin();
  BTSerial.begin(9600);
  //light begin
  lightMeter.begin();
  BTSerial.println(F("BH1750 Test"));
  // light end

  // 溫溼度begin
  Serial.begin(9600);
  BTSerial.println("DHTxx test!");
  dht.begin();
  // 溫溼度end

  // GPS begin
  gpsSerial.begin(GPSBaud);
  // GPS end

  // bluetooth begin
  BTSerial.println("Bluetooth is ready");
  // bluetooth end
}
float sensorVoltage;
float sensorValue;
int UV_index;
float lux;
float h, t, f, hif, hic;

void loop() {

  //purple begin
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue / 4;
  if (sensorVoltage < 10) {
    UV_index = 0;
  } else if (sensorVoltage <= 46) {
    UV_index = 1;
  } else if (sensorVoltage <= 65) {
    UV_index = 2;
  } else if (sensorVoltage <= 83) {
    UV_index = 3;
  } else if (sensorVoltage <= 103) {
    UV_index = 4;
  } else if (sensorVoltage <= 124) {
    UV_index = 5;
  } else if (sensorVoltage <= 142) {
    UV_index = 6;
  } else if (sensorVoltage <= 162) {
    UV_index = 7;
  } else if (sensorVoltage <= 180) {
    UV_index = 8;
  } else if (sensorVoltage <= 200) {
    UV_index = 9;
  } else if (sensorVoltage <= 221) {
    UV_index = 10;
  } else {
    UV_index = 11;
  }
  // purple end
  // light begin
  lux = lightMeter.readLightLevel();
  // light end
  // 溫溼度begin
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    BTSerial.println("Failed to read from DHT sensor!");
    return;
  }
  hif = dht.computeHeatIndex(f, h);
  hic = dht.computeHeatIndex(t, h, false);
  // 溫溼度end
  // GPS begin
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    }

  if (millis() > 10000 && gps.charsProcessed() < 20) {
    BTSerial.println("No GPS detected");
  }
  // GPS end
}

//GPS
void displayInfo() {

  BTSerial.print("UV index: ");
  BTSerial.print(UV_index);
  delay(500);

  String uv_string;
  if (UV_index >= 10) {
    uv_string = " (紫爆紫爆 勾 HOME ㄖㄨㄞˇ 鬧!)";
  } else if (UV_index < 10 && UV_index >= 3) {
    uv_string = " (記得做好防曬!)";
  } else {
    uv_string = " (SAFE!安全上壘!)";
  }
  BTSerial.print(uv_string);
  BTSerial.print("\n");
  delay(1000);

  BTSerial.print("Light: ");
  BTSerial.print(lux);
  BTSerial.print(" lx\n");
  delay(500);
  BTSerial.print("Humidity: ");
  BTSerial.print(h);
  delay(500);
  if (h > 70) {
    BTSerial.print(String("% (你知道清大下雨什麼最多嗎？...大~水~蟻~)"));
  }
  BTSerial.print("\n");
  BTSerial.print("Temperature: ");
  BTSerial.print(t);
  BTSerial.print(" °C");
  delay(1000);

  String temp_string1;
  if (t > 25) {
    temp_string1 = " (全國在悼念一個叫'熱'的傢伙，因為...熱死了!)";
  } else {
    temp_string1 = " (被當怎麼辦？重修就好)";
  }
  BTSerial.println(temp_string1);
 
  delay(1000);

  if (gps.location.isValid()) {
    BTSerial.print("Latitude: ");
    BTSerial.print(gps.location.lat(), 4);
    BTSerial.println(" °");
    BTSerial.print("Longitude: ");
    BTSerial.print(gps.location.lng(), 4);
    BTSerial.println(" °");
    BTSerial.print("Altitude: ");
    BTSerial.print(gps.altitude.meters());
    BTSerial.println(" m");
  } else {
    BTSerial.println("Location: Not Available");
  }
  delay(500);
  
  int gps_time_hour;
  BTSerial.print("Time: ");
  if (gps.time.isValid()) {
    gps_time_hour = gps.time.hour() + 8;
    if(gps_time_hour >= 24){
      gps_time_hour = gps_time_hour - 24;
    }
    if (gps_time_hour < 10) BTSerial.print(F("0"));
    BTSerial.print(gps_time_hour);
    BTSerial.print(":");
    if (gps.time.minute() < 10) BTSerial.print(F("0"));
    BTSerial.print(gps.time.minute());
    BTSerial.print(":");
    if (gps.time.second() < 10) BTSerial.print(F("0"));
    BTSerial.print(gps.time.second());
    BTSerial.print(" ");
  } else {
    BTSerial.println("Not Available");
  }

  String hour_string;
  if (gps_time_hour >= 5 && gps_time_hour < 12) {
    hour_string = "(早安，叫醒你的是鬧鐘，不是夢想。)";
  } else if (gps_time_hour >= 12 && gps_time_hour < 17) {
    hour_string = "(午安，平安，吃餅乾)";
  } else {
    hour_string = "(晚安，工作一天辛苦了^^)";
  }
   
  BTSerial.print(hour_string);
  BTSerial.print("\n");

  BTSerial.print("Date: ");
  int date_string;
  if (gps.date.isValid()) {
    BTSerial.print(gps.date.month());
    BTSerial.print("/");
    date_string = gps.date.day();
    if(gps.time.hour() >= 16){
      date_string = date_string + 1;
    }
    BTSerial.print(date_string);
    BTSerial.print("/");
    BTSerial.print(gps.date.year());
    
  } else {
    BTSerial.println("Not Available");
  }

   // reminder
  if (gps.date.month() == 6 && date_string == 12)
  {
    BTSerial.println(String(" (2023的今天，SpaceX 發射52顆低軌道衛星——星鏈 酷吧!)"));
  }
  else if (gps.date.month() == 8 && date_string == 15)
  {
    BTSerial.println(String(" (嫦娥在今天奔月 \n酷吧!)")); 
  }
  else if (gps.date.month() == 8 && date_string == 24)
  {
    BTSerial.println(String(" (2017的今天，福衛五號發射並成功與中壢地面站通聯 酷吧!)"));
  }
  else if (gps.date.month() == 10 && date_string == 3)
  {
    BTSerial.println(String(" (1991年的今天是TASA成立的日期 酷吧!)")); 
  }
  else if (gps.date.month() == 9 && date_string == 6)
  {
    BTSerial.println(String(" (今天是人造衛星節 酷吧!)"));
  }


  BTSerial.print("\n");
  delay(10000);
}
