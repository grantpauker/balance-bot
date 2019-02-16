#include <iostream>
#include "pid.hpp"

PID::PID(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd)
{
    pre_error = 0;
    integral = 0;
    min_output = 0;
    max_output = 0;
}
PID::PID(float kp, float ki, float kd, float min_output, float max_output) : kp(kp), ki(ki), kd(kd), min_output(min_output), max_output(max_output)
{
    pre_error = 0;
    integral = 0;
}

float PID::calculate(float dt, float sp, float pv)
{
    setpoint = sp;
    float error = setpoint - pv;
    float p_out = kp * error;
    integral += error * dt;
    float i_out = ki * integral;
    float d_out = kd * ((error - pre_error) / dt);
    float output = p_out + i_out + d_out;
    if (min_output != max_output)
    {
        output = BOUND(output, min_output, max_output);
    }
    return output;
}
void PID::reset()
{
    kp = 0;
    ki = 0;
    kd = 0;
    pre_error = 0;
    integral = 0;
}