#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "pca9685.hpp"

PCA9685::PCA9685(float freq)
{
  fd = wiringPiI2CSetup(PCA9685_I2C_ADDRESS);
  if (freq > 0)
  {
    setPWMFreq(freq);
  }
}

void PCA9685::setPWMFreq(int freq)
{
  freq = (freq > 1000 ? 1000 : (freq < 40 ? 40 : freq));

  int prescale = (int)(25000000.0f / (4096 * freq) - 0.5f);

  int settings = wiringPiI2CReadReg8(fd, PCA9685_MODE1) & 0x7F;
  int sleep = settings | 0x10;
  int wake = settings & 0xEF;
  int restart = wake | 0x80;

  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, sleep);
  wiringPiI2CWriteReg8(fd, PCA9685_PRESCALE, prescale);
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, wake);

  delay(1);
  wiringPiI2CWriteReg8(fd, PCA9685_MODE1, restart);
}

void PCA9685::setPWM(int pin, int on, int off)
{
  int reg = PIN(pin);
  wiringPiI2CWriteReg16(fd, reg, on & 0x0FFF);
  wiringPiI2CWriteReg16(fd, reg + 2, off & 0x0FFF);
}

void PCA9685::setPWM(int on, int off)
{
  wiringPiI2CWriteReg16(fd, PCA9685_ALL_LED_ON_L, on & 0x0FFF);
  wiringPiI2CWriteReg16(fd, PCA9685_ALL_LED_ON_L + 2, off & 0x0FFF);
}

void PCA9685::reset()
{
  wiringPiI2CWriteReg16(fd, PCA9685_ALL_LED_ON_L, 0x0);
  wiringPiI2CWriteReg16(fd, PCA9685_ALL_LED_ON_L + 2, 0x1000);
}

int *PCA9685::getPWM(int pin)
{
  int pwm[2];
  int reg = PIN(pin);
  pwm[0] = wiringPiI2CReadReg16(fd, reg);
  pwm[1] = wiringPiI2CReadReg16(fd, reg + 2);
  return pwm;
}
