#include <iostream>
#include <assert.h>
#include "pid.hpp"
#include "pca9685.hpp"
#include "bno055.hpp"
#include "i2c.hpp"
#include "l298n.hpp"

#define LOG(a, b) std::cout << a << ": " << b
#define LOGN(a, b) std::cout << a << ": " << b << "\n"

#define I2C_BUS_1 1
#define I2C_BUS_2 1
#define PWM_FREQ 50

#define L_MOTOR_IN1 1
#define L_MOTOR_IN2 2
#define L_MOTOR_ENABLE 3

#define R_MOTOR_IN1 4
#define R_MOTOR_IN2 5
#define R_MOTOR_ENABLE 6

#define KP 1
#define KI 0.001
#define KD 0.0
#define DT 2

int main()
{
    I2C i2c1(I2C_BUS_1);
    I2C i2c2(I2C_BUS_2);
    BNO055 imu(i2c1);
    assert(imu.init(OPERATION_MODE_NDOF));
    auto status = imu.getSystemStatus(true);
    LOGN("imu system status", status[0]);
    LOGN("imu self test result", status[1]);
    if (status[0] == 0x01)
    {
        LOGN("imu system error", status[2]);
    }
    PCA9685 pwm_driver(i2c2, PWM_FREQ);
    L298N left_motor(pwm_driver, L_MOTOR_ENABLE, L_MOTOR_IN1, L_MOTOR_IN2);
    L298N right_motor(pwm_driver, R_MOTOR_ENABLE, R_MOTOR_IN1, R_MOTOR_IN2);
    PID pid(KP, KI, KD);

    while (1)
    {
        float pitch = imu.readEuler()[1];
        float output = pid.calculate(DT, 0, pitch);
        left_motor.setSpeed(output);
        right_motor.setSpeed(output);
        std::cout << pitch << "\t" << output << "\n";
        I2C::delay(DT);
    }
}