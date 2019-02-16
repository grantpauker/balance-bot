#include <iostream>
#include "pid.hpp"
#include "pca9685.hpp"

int main()
{
    PCA9685 pwmdriver(50);
    pwmdriver.setPWM(0, 1024, 3072);
    for (int pin = 0; pin < 16; pin++)
    {
        printf("PWM Pin %d: %d/%d", pin, pwmdriver.getPWMOn(pin), pwmdriver.getPWMOff(pin));
    }
    return 0;
}