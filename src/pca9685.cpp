#include <iostream>
#include <array>
#include <algorithm>
#include "pca9685.hpp"
#include "i2c.hpp"

PCA9685::PCA9685(I2C i2c, float freq) : i2c(i2c)
{
    i2c.init(PCA9685_I2C_ADDRESS);
    if (freq > 0)
    {
        setPWMFreq(freq);
    }
}

void PCA9685::setPWMFreq(int freq)
{
    freq = (freq > 1000 ? 1000 : (freq < 40 ? 40 : freq));

    int prescale = (int)(25000000.0f / (4096 * freq) - 0.5f);

    int settings = i2c.readByte(PCA9685_MODE1) & 0x7F;
    int sleep = settings | 0x10;
    int wake = settings & 0xEF;
    int restart = wake | 0x80;

    i2c.writeByte(PCA9685_MODE1, sleep);
    i2c.writeByte(PCA9685_PRESCALE, prescale);
    i2c.writeByte(PCA9685_MODE1, wake);

    i2c.delay(5);
    i2c.writeByte(PCA9685_MODE1, restart);
}

void PCA9685::setPWM(int pin, int on, int off)
{
    int reg = PIN(pin);
    i2c.writeByte(reg, on & 0x0FFF);
    i2c.writeByte(reg + 2, off & 0x0FFF);
}

void PCA9685::setPWM(int pin, int val)
{
    int reg = PIN(pin);
    val = std::min(val, 4095);
    if (val == 0)
    {
        setPWM(pin, 0, 4096);
    }
    else if (val == 4095)
    {
        setPWM(pin, 4096, 0);
    }
    else
    {
        setPWM(pin, 0, val);
    }
}

std::array<int, 2> PCA9685::getPWM(int pin)
{
    int reg = PIN(pin);
    std::array<int, 2> ret;
    ret[0] = i2c.readByte(reg);
    ret[1] = i2c.readByte(reg + 2);
    return ret;
}

void PCA9685::reset()
{
    i2c.writeByte(PCA9685_ALL_LED_ON_L, 0x0);
    i2c.writeByte(PCA9685_ALL_LED_ON_L + 2, 0x1000);
}
