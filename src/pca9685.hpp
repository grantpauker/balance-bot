#ifndef H_PCA9685
#define H_PCA9685

#define PCA9685_I2C_ADDRESS 0x40
#define PCA9685_MODE1 0x00
#define PCA9685_PRESCALE 0xFE
#define PCA9685_LED0_ON_L 0x6
#define PCA9685_ALL_LED_ON_L 0xFA

#define PIN(p) PCA9685_LED0_ON_L + 4 * p

class PCA9685
{
public:
  PCA9685(float);
  void setPWMFreq(int);
  void setPWM(int, int, int);
  void setPWM(int, int);
  int getPWMOn(int);
  int getPWMOff(int);

  void reset();

private:
  int fd;
};
#endif