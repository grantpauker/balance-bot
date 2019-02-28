#include <iostream>
#include <assert.h>
#include "pid.hpp"
#include "pca9685.hpp"
#include "bno055.hpp"
#include "i2c.hpp"
#include "l298n.hpp"
#include "motorcontroller.hpp"

#define LOG(a, b) std::cout << a << ": " << b
#define LOGN(a, b) std::cout << a << ": " << b << "\n"

#define I2C_BUS 1
#define PWM_FREQ 50

#define L_MOTOR_IN1 0
#define L_MOTOR_IN2 2
#define L_MOTOR_ENABLE 0

#define R_MOTOR_IN1 15
#define R_MOTOR_IN2 16
#define R_MOTOR_ENABLE 1

#define KP 45.5
#define KI 0.0
#define KD 0.0
#define DT 2

int main()
{
    I2C i2c(I2C_BUS);
    i2c.enable(BNO055_ADDRESS_A);
    BNO055 imu(i2c);
    assert(imu.init(OPERATION_MODE_NDOF));
    auto status = imu.getSystemStatus(true);
    LOGN("imu system status", status[0]);
    LOGN("imu self test result", status[1]);
    if (status[0] == 0x01)
    {
        LOGN("imu system error", status[2]);
    }
    PCA9685 pwm_driver(i2c, PWM_FREQ);
    L298N left_l298n(pwm_driver, L_MOTOR_ENABLE, L_MOTOR_IN1, L_MOTOR_IN2);
    L298N right_l298n(pwm_driver, R_MOTOR_ENABLE, R_MOTOR_IN1, R_MOTOR_IN2);
    MotorController left_motor(left_l298n);
    MotorController right_motor(right_l298n);
    PID pid(KP, KI, KD, -4096, 4096);
    float output;
    float pitch;
    while (1)
    {
        i2c.enable(BNO055_ADDRESS_A);
        pitch = imu.readEuler()[1];
        output = pid.calculate(DT, 0, pitch);
        i2c.enable(PCA9685_ADDRESS);
        left_motor.setSpeed(output);
        right_motor.setSpeed(output);
        std::cout << pitch << "\t" << output << "\n";
        I2C::delay(DT);
    }
}