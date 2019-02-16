#include <iostream>
#include "pid.hpp"

int main()
{
    PID controller = PID(1, 0, 0);
    float output = controller.calculate(10, 0, 0);
    return 0;
}