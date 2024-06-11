// light begin
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;
// light end


//溫溼度begin

#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
// 溫溼度end

// bluetooth start
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10,11); /* Pin 10 -->  TX, Pin 11 -->  RX */
char bluetoothData;
int led = 13;
bool ledState;
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
  
//light begin
  Wire.begin();
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
    BTSerial.begin(9600);
  pinMode(led, OUTPUT);
    BTSerial.println("Bluetooth is ready");
// bluetooth end
}
float sensorVoltage; 
float sensorValue;
int UV_index;
float lux;
float h,t,f, hif, hic;

void loop() {
  
  //purple begin
  
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/4;
  if (sensorVoltage < 10){
    UV_index=0;
  }
  else if (sensorVoltage <= 46){
    UV_index=1;
  }
  else if (sensorVoltage <= 65){
    UV_index=2;
  }
  else if (sensorVoltage <= 83){
    UV_index=3;
  }
  else if (sensorVoltage <= 103){
    UV_index=4;
  }
  else if (sensorVoltage <= 124){
    UV_index=5;
  }
  else if (sensorVoltage <= 142){
    UV_index=6;
  }
  else if (sensorVoltage <= 162){
    UV_index=7;
  }
  else if (sensorVoltage <= 180){
    UV_index=8;
  }
  else if (sensorVoltage <= 200){
    UV_index=9;
  }
  else if (sensorVoltage <= 221){
    UV_index=10;
  }
  else {
    UV_index=11;
  }
  //delay(250);
  // purple end
  // light begin
  lux = lightMeter.readLightLevel();
  delay(1000);
  // light end

  // 溫溼度begin
  //delay(1000);
  
  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    BTSerial.println("Failed to read from DHT sensor!");
    return;
  }

  hif = dht.computeHeatIndex(f, h);
  hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%\n");

  //BTSerial.println("");
  // 溫溼度end


  // GPS begin
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  if (millis() > 10000 && gps.charsProcessed() < 20)
  {
    BTSerial.println("No GPS detected");
    //while(true);
  }
  // GPS end
  
}

//GPS
void displayInfo()
{
  
  BTSerial.print("UV index: ");
  BTSerial.println(UV_index);

  if (UV_index >= 10) {
    //println("紫爆紫爆 勾 HOME ㄖㄨㄞˇ 鬧!!");
    BTSerial.println("\u7d2b\u7206!\u7d2b\u7206! \u52fe HOME \u3116\u3128\u311eV \u9b27!!");
  }
  else if (UV_index < 10 && UV_index >= 3) {
    //記得帶傘出門，做好防曬喔！
    BTSerial.println("\u8a18\u5f97\u5e36\u5098\u51fa\u9580\uff0c\u505a\u597d\u9632\u66ec\u5594\uff01");
  }
  else {
    //SAFE！ 安全上壘！
    BTSerial.println("SAFE\uff01 \u5b89\u5168\u4e0a\u58d8\uff01");
  }

  
  BTSerial.print("Light: ");
  BTSerial.print(lux);
  BTSerial.println(" lx");
 
  BTSerial.print("Humidity: ");
  BTSerial.print(h);
  BTSerial.print("%\n");
  if (h > 80) {
    //println("你知道清大下雨的時候什麼最多嗎？  大~水~蟻~~~");
    BTSerial.println("\u4f60\u77e5\u9053\u6e05\u5927\u4e0b\u96e8\u7684\u6642\u5019\u4ec0\u9ebc\u6700\u591a\u55ce\uff1f\u0020\u0020\u5927\u007e\u6c34\u007e\u87fb\u007e\u007e\u007e");
  }
  BTSerial.print("Temperature: ");
  BTSerial.print(t);
  BTSerial.println(" °C ");

  if (t > 30) {
    //今天太熱了，講個冷笑話吧！如今全國都在悼念一個叫'熱'的傢伙，因為－－－－熱死了！！！！！
    BTSerial.println("\u4eca\u5929\u592a\u71b1\u4e86\uff0c\u8b1b\u500b\u51b7\u7b11\u8a71\u5427\uff01");
    BTSerial.println("\u5982\u4eca\u5168\u570b\u90fd\u5728\u60bc\u5ff5\u4e00\u500b\u53eb'\u71b1'\u7684\u50a2\u4f19\uff0c\u56e0\u70ba\uff0d\uff0d\uff0d\uff0d");
    BTSerial.println("\u71b1\u6b7b\u4e86\uff01\uff01\uff01\uff01\uff01");
  }
  else if (t < 15) {
    // 今天太冷了，講個冷笑話吧！這麼冷的天氣，冰塊最想做什麼？"退伍，因為它當冰（兵）很久了。 哈 哈 哈 哈 哈
    BTSerial.println("\u4eca\u5929\u592a\u51b7\u4e86\uff0c\u8b1b\u500b\u51b7\u7b11\u8a71\u5427\uff01");
    BTSerial.println("\u9019\u9ebc\u51b7\u7684\u5929\u6c23\uff0c\u51b0\u584a\u6700\u60f3\u505a\u4ec0\u9ebc\uff1f");
    BTSerial.println("\u9000\u4f0d\uff0c\u56e0\u70ba\u5b83\u7576\u51b0\uff08\u5175\uff09\u5f88\u4e45\u4e86\u3002 \u54c8 \u54c8 \u54c8 \u54c8 \u54c8");
  }
  else {
    // 今天是個適合講冷笑話的日子，講個冷笑話吧！被當怎麼辦？重修就好
    BTSerial.println("\u4eca\u5929\u662f\u500b\u9069\u5408\u8b1b\u51b7\u7b11\u8a71\u7684\u65e5\u5b50\uff0c\u8b1b\u500b\u51b7\u7b11\u8a71\u5427\uff01");
    BTSerial.println("\u88ab\u7576\u600e\u9ebc\u8fa6\uff1f");
    BTSerial.println("\u91cd\u4fee\u5c31\u597d");
  }
 


  
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

    // reminder
    if (gps.date.month() == 6 && gps.date.day() == 11)
    {
      // 你知道2023的今天，SpaceX 發射52顆低軌道衛星——星鏈嗎？　酷吧"
      BTSerial.println("\u4f60\u77e5\u90532023\u7684\u4eca\u5929\uff0cSpaceX \u767c\u5c0452\u9846\u4f4e\u8ecc\u9053\u885b\u661f\u2014\u2014\u661f\u93c8\u55ce\uff1f\u3000\u9177\u5427");
    }
    else if (gps.date.month() == 8 && gps.date.day() == 15)
    {
      // 你知道嫦娥在今天奔月嗎？
      BTSerial.println("\u4f60\u77e5\u9053\u5ae6\u5a25\u5728\u4eca\u5929\u5954\u6708\u55ce\uff1f");
    }
    else if (gps.date.month() == 8 && gps.date.day() == 24)
    {
      // 你知道2017的今天，福衛五號發射並成功與中壢地面站通聯嗎？
      BTSerial.println("\u4f60\u77e5\u90532017\u7684\u4eca\u5929\uff0c\u798f\u885b\u4e94\u865f\u767c\u5c04\u4e26\u6210\u529f\u8207\u4e2d\u58e2\u5730\u9762\u7ad9\u901a\u806f\u55ce\uff1f");
    }
    else if (gps.date.month() == 10 && gps.date.day() == 3)
    {
      // 你知道1991年的今天是TASA成立的日期嗎？
      BTSerial.println("\u4f60\u77e5\u90531991\u5e74\u7684\u4eca\u5929\u662fTASA\u6210\u7acb\u7684\u65e5\u671f\u55ce\uff1f");
    }
    else if (gps.date.month() == 9 && gps.date.day() == 6)
    {
      // 你知道今天是人造衛星節嗎？
      BTSerial.println("\u4f60\u77e5\u9053\u4eca\u5929\u662f\u4eba\u9020\u885b\u661f\u7bc0\u55ce\uff1f");
    }

    
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

    // reminder
    if (gps.time.hour() >= 5 && gps.time.hour() < 12)
    {
      // 早安～叫醒你的是鬧鐘，不是夢想。
      BTSerial.println("\u65e9\u5b89\uff0c\u53eb\u9192\u4f60\u7684\u662f\u9b27\u9418\uff0c\u4e0d\u662f\u5922\u60f3\u3002");
    }
    else if (gps.time.hour() >= 12 && gps.time.hour() < 18)
    {
      // 午安，平安，吃餅乾。
      BTSerial.println("\u5348\u5b89\uff0c\u5e73\u5b89\uff0c\u5403\u9905\u4e7e\u3002");
    }
    else
    {
      // 太陽要下山了，工作一天辛苦了。
      BTSerial.println("\u592a\u967d\u8981\u4e0b\u5c71\u4e86\uff0c\u5de5\u4f5c\u4e00\u5929\u8f9b\u82e6\u4e86\u3002");
    }

  }
  else
  {
    BTSerial.println("Not Available");
  }

  BTSerial.println();
  BTSerial.println();
  delay(1000);
}
