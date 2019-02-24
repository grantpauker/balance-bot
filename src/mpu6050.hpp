#ifndef H_MPU6050
#define H_MPU6050

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_I2C_ADDRESS 0x68

#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_ZOUT_H 0x47

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_ZOUT_H 0x3F

#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

#define MPU6050_FS_SEL_0 0 // +/- 250 deg/sec
#define MPU6050_FS_SEL_1 1 // +/- 500 deg/sec
#define MPU6050_FS_SEL_2 2 // +/- 1000 deg/sec
#define MPU6050_FS_SEL_3 3 // +/- 2000 deg/sec

#define MPU6050_AFS_SEL_0 0 // +/- 2 g
#define MPU6050_AFS_SEL_1 1 // +/- 4 g
#define MPU6050_AFS_SEL_2 2 // +/- 8 g
#define MPU6050_AFS_SEL_3 3 // +/- 16 g

class MPU6050
{
public:
  MPU6050(int);
  void calibrate();
  void configGyro(int);
  void configAccel(int);

  float getGyroX();
  float getGyroY();
  float getGyroZ();
  float getAccelX();
  float getAccelY();
  float getAccelZ();
  void getPitchRoll(float *, float *);
  int dt;

private:
  int read_word(int);
  int fd;
  int accel_x_offset = 0;
  int accel_y_offset = 0;
  int accel_z_offset = 0;
  int gyro_x_offset = 0;
  int gyro_y_offset = 0;
  int gyro_z_offset = 0;
  float gyro_sensitivities[4] = {131.0f, 65.5f, 32.8f, 16.4f};          // LSB/(deg/sec)
  float accel_sensitivities[4] = {16384.0f, 8192.0f, 4096.0f, 2048.0f}; // LSB/g
  float gyro_sensitivity;
  float accel_sensitivity;
};
#endif