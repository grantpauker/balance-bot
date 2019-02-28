#ifndef H_MOTORCONTROLLER
#define H_MOTORCONTROLLER
#include "l298n.hpp"
class MotorController
{
  public:
    MotorController(L298N);
    MotorController(L298N, bool);
    void setSpeed(int);
    void stop();
    int getSpeed();
    bool inverted;

  private:
    L298N l298n;
    int speed;
};
#endif
