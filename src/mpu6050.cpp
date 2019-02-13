#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "mpu6050.hpp"

MPU6050::MPU6050()
{
  fd = wiringPiI2CSetup(MPU6050_I2C_ADDRESS);
  wiringPiI2CReadReg8(fd, MPU6050_PWR_MGMT_1);
  wiringPiI2CWriteReg16(fd, MPU6050_PWR_MGMT_1, 0);
}

int MPU6050::getGyroX()
{
  return wiringPiI2CReadReg8(fd, MPU6050_GYRO_XOUT_H);
}

int MPU6050::getGyroY()
{
  return wiringPiI2CReadReg8(fd, MPU6050_GYRO_XOUT_H);
}

int MPU6050::getGyroZ()
{
  return wiringPiI2CReadReg8(fd, MPU6050_GYRO_XOUT_H);
}

int MPU6050::getAccelX()
{
  return wiringPiI2CReadReg8(fd, MPU6050_GYRO_XOUT_H);
}

int MPU6050::getAccelY()
{
  return wiringPiI2CReadReg8(fd, MPU6050_GYRO_XOUT_H);
}

int MPU6050::getAccelZ()
{
  return wiringPiI2CReadReg8(fd, MPU6050_GYRO_XOUT_H);
}
