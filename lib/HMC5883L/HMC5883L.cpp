#include "HMC5883L.h"

rawMagData HMC5883L::getRawData(){
  rawMagData data;
  data.x = (read16(HMC58831_REGISTER_DATA_X_MSB) - xOffset) * xScale;
  data.z = (read16(HMC58831_REGISTER_DATA_Z_MSB) - zOffset) * zScale;
  data.y = (read16(HMC58831_REGISTER_DATA_Y_MSB) - yOffset) * yScale;
  return data;
}

magData HMC5883L::getMagData(){
  magData data;
  data.rawData = getRawData();
  data.heading = calculateHeading(data.rawData.x, data.rawData.y);
  return data;
}

void HMC5883L::beginSelfTest(hmc58831_self_test test){
  int value = read8(HMC58831_REGISTER_CONFIGURATION_A);
  value &= 0b11111100;
  value |= test;
  write8(HMC58831_REGISTER_CONFIGURATION_A,value);
  delay(10);
  //read 3 times to let the mag update
  for(int i = 0; i < 3; i ++){
    getRawData();
    delay(5);
  }
}

void HMC5883L::setAxisOffsets(int x, int y, int z){
  xOffset = x;
  yOffset = y;
  zOffset = z;
}

void HMC5883L::setAxisScales(double x, double y, double z){
  xScale = x;
  yScale = y;
  zScale = z;
}

void HMC5883L::endSelfTest(){
  int value = read8(HMC58831_REGISTER_CONFIGURATION_A);
  value &= 0b11111100;
  value |= HMC5883L_SELF_TEST_NONE;
  write8(HMC58831_REGISTER_CONFIGURATION_A,value);
  delay(10);
  //read 3 times to let the mag update
  for(int i = 0; i < 3; i ++){
    getRawData();
    delay(5);
  }
}

float HMC5883L::calculateHeading(float x, float y){
  float heading = atan2(y , x) *180.0 / M_PI;
  if(heading < 0) heading += 360;
  else if(heading > 360) heading -= 360;
  return heading;
}



int HMC5883L::begin(){
  Wire.begin();
  if(!checkDevice()){
    return -1;
  }
  setDataRange(HMC5883L_RANGE_1_3GA);
  //beginSelfTest(HMC5883L_SELF_TEST_NEGATIVE);
  setMode(HMC58831_CONTINUOUS);
  setSamplesPerRead(HMC5883L_SAMPLES_8);
  setDataRate(HMC5883L_DATARATE_30HZ);
  return 0;
}

void HMC5883L::setMode(hmc58831_read_mode mode){
  int value = read8(HMC58831_REGISTER_READ_MODE);
  value &= 0b11111100;
  value |= mode;
  write8(HMC58831_REGISTER_READ_MODE, value);
}

void HMC5883L::setSamplesPerRead(hmc58831_samples_per_read samples){
  int value = read8(HMC58831_REGISTER_CONFIGURATION_A);
  value &= 0b10011111;
  value |= (samples << 5);
  write8(HMC58831_REGISTER_CONFIGURATION_A,value);
}

void HMC5883L::setDataRange(hmc58831_range range){
  write8(HMC58831_REGISTER_CONFIGURATION_B,range << 5);
}

void HMC5883L::setDataRate(hmc58831_data_rate rate){
  int value = read8(HMC58831_REGISTER_CONFIGURATION_A);
  value &= 0b11100011;
  value |= (rate << 2);
  write8(HMC58831_REGISTER_CONFIGURATION_A,value);
}


void HMC5883L::write8(int reg, int value){
  Wire.beginTransmission(hmc5883Address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void HMC5883L::write8(int reg){
  Wire.beginTransmission(hmc5883Address);
  Wire.write(reg);
  Wire.endTransmission();
}

int HMC5883L::read8(int reg){
  write8(reg);
  Wire.requestFrom(hmc5883Address,1);
  if(Wire.available() >= 1){
    return Wire.read();
  }
  return -1;
}

int HMC5883L::read16(int regLow){
  write8(regLow);
  Wire.requestFrom(hmc5883Address,2);
  int value = -1;
  if(Wire.available() >= 2){
    value = Wire.read();
    value = value << 8;
    value |= Wire.read();
  }
  return value;
}


bool HMC5883L::checkDevice(){
  if(read8(HMC58831_REGISTER_ID_A) != 0x48 || read8(HMC58831_REGISTER_ID_B) != 0x34 || read8(HMC58831_REGISTER_ID_C) != 0x33){
    return false;
  }
  return true;
}
