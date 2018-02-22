#ifndef HMC5883L_H
#define HMC5883L_H

#include "Arduino.h"
#include "Wire.h"

#define HMC58831_REGISTER_CONFIGURATION_A   0x00
#define HMC58831_REGISTER_CONFIGURATION_B   0x01
#define HMC58831_REGISTER_READ_MODE         0x02
#define HMC58831_REGISTER_DATA_X_MSB        0x03
#define HMC58831_REGISTER_DATA_X_LSB        0x04
#define HMC58831_REGISTER_DATA_Y_MSB        0x07
#define HMC58831_REGISTER_DATA_Y_LSB        0x08
#define HMC58831_REGISTER_DATA_Z_MSB        0x05
#define HMC58831_REGISTER_DATA_Z_LSB        0x06
#define HMC58831_REGISTER_STATUS            0x09
#define HMC58831_REGISTER_ID_A              0x0A
#define HMC58831_REGISTER_ID_B              0x0B
#define HMC58831_REGISTER_ID_C              0x0C


enum hmc58831_data_rate {
  HMC5883L_DATARATE_75HZ       = 0b110,
  HMC5883L_DATARATE_30HZ       = 0b101,
  HMC5883L_DATARATE_15HZ       = 0b100,
  HMC5883L_DATARATE_7_5HZ      = 0b011,
  HMC5883L_DATARATE_3HZ        = 0b010,
  HMC5883L_DATARATE_1_5HZ      = 0b001,
  HMC5883L_DATARATE_0_75_HZ    = 0b000
};

enum hmc58831_read_mode {
  HMC58831_CONTINUOUS             = 0x00,
  HMC58831_SINGLE                 = 0x01,
  HMC58831_IDLE                   = 0x02
};

enum hmc58831_range{
  HMC5883L_RANGE_8_1GA     = 0b111,
  HMC5883L_RANGE_5_6GA     = 0b110,
  HMC5883L_RANGE_4_7GA     = 0b101,
  HMC5883L_RANGE_4GA       = 0b100,
  HMC5883L_RANGE_2_5GA     = 0b011,
  HMC5883L_RANGE_1_9GA     = 0b010,
  HMC5883L_RANGE_1_3GA     = 0b001,
  HMC5883L_RANGE_0_88GA    = 0b000
};

enum hmc58831_samples_per_read {
  HMC5883L_SAMPLES_1            = 0x00,
  HMC5883L_SAMPLES_2            = 0x01,
  HMC5883L_SAMPLES_4            = 0x10,
  HMC5883L_SAMPLES_8            = 0x11
};







enum hmc58831_self_test{
  HMC5883L_SELF_TEST_POSITIVE = 0x01,
  HMC5883L_SELF_TEST_NEGATIVE = 0x02,
  HMC5883L_SELF_TEST_NONE = 0x00
};


struct rawMagData{
  int x = 0;
  int y = 0;
  int z = 0;
};

struct magData{
  float heading = 0;
  rawMagData rawData;
};


class HMC5883L{
  public:
    int begin();
    void setMode(hmc58831_read_mode mode);
    void setSamplesPerRead(hmc58831_samples_per_read samples);
    void setDataRange(hmc58831_range range);
    void setDataRate(hmc58831_data_rate rate);
    rawMagData getRawData();
    magData getMagData();
    void beginSelfTest(hmc58831_self_test test);
    void endSelfTest();
    void setAxisOffsets(int x, int y, int z);
    void setAxisScales(double x, double y, double z);
  private:
    void write8(int reg, int value);
    void write8(int reg);
    int read8(int reg);
    int read16(int reg);
    bool checkDevice();
    int xOffset = 0;
    double xScale = 1.0;
    int yOffset = 0;
    double yScale = 1.0;
    int zOffset = 0;
    double zScale = 1.0;
    float calculateHeading(float x, float y);
    const int hmc5883Address = 0x1E; //0011110b, I2C 7bit address of HMC5883

};

#endif
