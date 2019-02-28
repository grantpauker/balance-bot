#ifndef H_PCA9685
#define H_PCA9685

#include <array>
#include "i2c.hpp"

#define PCA9685_ADDRESS 0x40
#define PCA9685_MODE1 0x00
#define PCA9685_PRESCALE 0xFE
#define PCA9685_LED0_ON_L 0x6
#define PCA9685_ALL_LED_ON_L 0xFA

#define PIN(p) p != 16 ? PCA9685_LED0_ON_L + 4 * p : PCA9685_ALL_LED_ON_L

class PCA9685
{
public:
  PCA9685(I2C, float);
  void setPWMFreq(int);
  void setPWM(int, int, int);
  void setPWM(int, int);
  std::array<int, 2> getPWM(int);

  void reset();

private:
  I2C i2c;
};
#endif