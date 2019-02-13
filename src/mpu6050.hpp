#ifndef H_MPU6050
#define H_MPU6050

#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_ZOUT_H 0x47

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_ZOUT_H 0x3F

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_I2C_ADDRESS 0x68

class MPU6050
{
  public:
    MPU6050();
    int getGyroX();
    int getGyroY();
    int getGyroZ();
    int getAccelX();
    int getAccelY();
    int getAccelZ();

  private:
    int fd;
};
#endif