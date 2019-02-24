#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cmath>
#include <iostream>
#include <string>
#include "mpu6050.hpp"

#define DEGREES(r) (r * 180.0 / M_PI)
#define RADIANS(d) (d * M_PI / 180.0)
#define READ_WORD(fd, reg) (wiringPiI2CReadReg8(fd, reg) << 8 | wiringPiI2CReadReg8(fd, reg + 1))

MPU6050::MPU6050(int dt) : dt(dt)
{
  fd = wiringPiI2CSetup(MPU6050_I2C_ADDRESS);
  if (fd < 0)
  {
    std::cout << "Error: fd is less than 0 (" + std::to_string(fd) + ")\n";
  }

  wiringPiI2CReadReg8(fd, MPU6050_PWR_MGMT_1);
  wiringPiI2CWriteReg16(fd, MPU6050_PWR_MGMT_1, 0);
  gyro_sensitivity = gyro_sensitivities[0];
  accel_sensitivity = accel_sensitivities[0];
}

void MPU6050::configGyro(int fs_sel)
{
  gyro_sensitivity = gyro_sensitivities[fs_sel];
  wiringPiI2CWriteReg8(fd, MPU6050_GYRO_CONFIG, fs_sel << 3);
}

void MPU6050::configAccel(int afs_sel)
{
  accel_sensitivity = accel_sensitivities[afs_sel];
  wiringPiI2CWriteReg8(fd, MPU6050_ACCEL_CONFIG, afs_sel << 3);
}

void MPU6050::calibrate()
{
  int samples = 100;
  long accel_x_sum, accel_y_sum, accel_z_sum = 0;
  long gyro_x_sum, gyro_y_sum, gyro_z_sum = 0;
  std::cout << "Calibrating gyro...\n";
  for (int i = 0; i < samples + 100; i++)
  {
    if (i < 100) //Ignore 1st 100 values
    {
      getAccelX();
      getAccelY();
      getAccelZ();
      getGyroX();
      getGyroY();
      getGyroZ();
      delay(2);
    }
    else
    {
      accel_x_sum += getAccelX();
      accel_y_sum += getAccelY();
      accel_z_sum += getAccelZ();
      gyro_x_sum += getGyroX();
      gyro_y_sum += getGyroY();
      gyro_z_sum += getGyroZ();
      delay(2);
    }
  }
  accel_x_offset = accel_x_sum / samples;
  accel_y_offset = accel_y_sum / samples;
  accel_z_offset = accel_z_sum / samples;
  gyro_x_offset = gyro_x_sum / samples;
  gyro_y_offset = gyro_y_sum / samples;
  gyro_z_offset = gyro_z_sum / samples;
  std::cout << gyro_x_offset << "Done calibrating gyro.\n";
}

float MPU6050::getGyroX()
{
  return (READ_WORD(fd, MPU6050_GYRO_XOUT_H)) / gyro_sensitivity - 3992;
}

float MPU6050::getGyroY()
{
  return (READ_WORD(fd, MPU6050_GYRO_YOUT_H)) / gyro_sensitivity - gyro_y_offset;
}

float MPU6050::getGyroZ()
{
  return (READ_WORD(fd, MPU6050_GYRO_ZOUT_H)) / gyro_sensitivity - gyro_z_offset;
}

float MPU6050::getAccelX()
{
  return (READ_WORD(fd, MPU6050_ACCEL_XOUT_H)) / accel_sensitivity - accel_x_offset;
}

float MPU6050::getAccelY()
{
  return (READ_WORD(fd, MPU6050_ACCEL_YOUT_H)) / accel_sensitivity - accel_y_offset;
}

float MPU6050::getAccelZ()
{
  return (READ_WORD(fd, MPU6050_ACCEL_ZOUT_H)) / accel_sensitivity - accel_z_offset;
}

void MPU6050::getPitchRoll(float *pitch, float *roll)
{
  float gx = getGyroX() * dt;
  float gy = getGyroY() * dt;
  float gz = getGyroZ() * dt;
  float ax = getAccelX();
  float ay = getAccelY();
  float az = getAccelZ();
  float a_pitch = atan2(ay, hypot(ax, az));
  float a_roll = atan2(-ax, az);
  // *pitch = ax;
  // *roll = ay;

  *pitch += gx;
  *roll -= gy;
  // int weight = 0.98;
  // *pitch = *pitch * weight + a_pitch * (1 - weight);
  // *roll = *roll * weight + a_roll * (1 - weight);
  return;
}
