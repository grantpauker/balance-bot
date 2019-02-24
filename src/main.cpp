#include <iostream>
#include <assert.h>
#include "pid.hpp"
#include "pca9685.hpp"
#include "bno055.hpp"

#define LOG(a, b) std::cout << a << ": " << b << "\n"

int main()
{
    BNO055 imu;
    assert(imu.init(OPERATION_MODE_NDOF));
    auto status = imu.getSystemStatus(true);
    LOG("system status", status[0]);
    LOG("self test result", status[0]);
    if (status[0] == 0x01)
    {
        LOG("system error", status[2]);
    }
    auto rev = imu.getRevision();
    LOG("software version", rev[0]);
    LOG("bootloader version", rev[1]);
    LOG("accelerometer id", rev[2]);
    LOG("magnetometer id", rev[3]);
    LOG("gyroscope id", rev[4]);
    while (1)
    {
        auto euler = imu.readEuler();
        auto cal_status = imu.getCalibrationStatus();
        LOG("yaw", euler[0]);
        LOG("pitch", euler[1]);
        LOG("roll", euler[2]);
        LOG("sys calibration", cal_status[0]);
        LOG("gyro calibration", cal_status[1]);
        LOG("accel calibration", cal_status[2]);
        LOG("mag calibration", cal_status[3]);
    }
}