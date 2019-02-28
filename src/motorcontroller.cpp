#include "motorcontroller.hpp"
#include "l298n.hpp"

MotorController::MotorController(L298N l298n) : l298n(l298n)
{
    inverted = false;
}

MotorController::MotorController(L298N l298n, bool inverted) : l298n(l298n), inverted(inverted)
{
}

void MotorController::setSpeed(int _speed)
{
    speed = inverted ? -_speed : _speed;
    l298n.setSpeed(speed);
}

void MotorController::stop()
{
    l298n.stop();
}

int MotorController::getSpeed()
{
    return speed;
}