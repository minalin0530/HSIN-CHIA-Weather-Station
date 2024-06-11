void setup() {
  Serial.begin(9600);
}

void loop() {
  float sensorVoltage; 
  float sensorValue;
  
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
  Serial.print("UV index: ");
  Serial.println(UV_index);
  delay(1000);
}
