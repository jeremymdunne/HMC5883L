#include <Arduino.h>
#include "HMC5883L.h"

HMC5883L compass;
rawMagData data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("INITED");
  if(compass.begin() != 0){
    Serial.println("Compass init fail! Check wiring!");
    while(true);
  }
  Serial.println("Compass init success!");
  compass.setAxisOffsets(-115, 95, 6);
  compass.setAxisScales(1.0, 1.0, 1.043);
  delay(10);
  compass.beginSelfTest(HMC5883L_SELF_TEST_NONE);
  delay(10);
}

magData mag;

void loop() {
  mag = compass.getMagData();
  String line =  "X:" + String(mag.rawData.x) + ";Y:" + String(mag.rawData.y) + ";Z:" + String(mag.rawData.z) + ";";
  Serial.println("Heading: " + String(mag.heading) + " raw:" + line);
  delay(100);
}
