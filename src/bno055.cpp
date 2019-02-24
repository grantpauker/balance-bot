#include <array>
#include <vector>
#include <algorithm>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <bno055.hpp>
BNO055::BNO055()
{
    fd = wiringPiI2CSetup(BNO055_ADDRESS_A);
}

void BNO055::writeByte(int reg, int data)
{
    wiringPiI2CWriteReg8(fd, reg, data);
}

int BNO055::readByte(int reg)
{
    return wiringPiI2CReadReg8(fd, reg);
}

int BNO055::readSignedByte(int reg)
{
    int data = wiringPiI2CReadReg8(fd, reg);
    return data <= 127 ? data : data - 256;
}

void BNO055::writeBytes(int reg, std::vector<int> data)
{
    for (int i = 0; i < data.size(); i++)
    {
        wiringPiI2CWriteReg8(fd, reg + i, data.at(i));
    }

}
std::vector<int> BNO055::readBytes(int reg, int count)
{
    std::vector<int> ret;
    for (int i = 0; i < count; i++)
    {
        ret.push_back(wiringPiI2CReadReg8(fd, reg + i));
    }

    return ret;
}

bool BNO055::init(int _mode)
{
    mode = _mode;
    writeByte(BNO055_PAGE_ID_ADDR, 0);
    configMode();
    writeByte(BNO055_PAGE_ID_ADDR, 0);
    int bno_id = readByte(BNO055_CHIP_ID_ADDR);
    if (bno_id != BNO055_ID)
    {
        return false;
    }

    writeByte(BNO055_SYS_TRIGGER_ADDR, 0x20);
    delay(650);
    writeByte(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
    writeByte(BNO055_SYS_TRIGGER_ADDR, 0x00);
    return true;
}

void BNO055::setMode(int mode)
{
    writeByte(BNO055_OPR_MODE_ADDR, mode & 0xFF);
    delay(30);
}

void BNO055::configMode()
{
    setMode(OPERATION_MODE_CONFIG);
}

void BNO055::operationMode()
{
    setMode(mode);
}

std::array<int, 5> BNO055::getRevision()
{
    int accel = readByte(BNO055_ACCEL_REV_ID_ADDR);
    int mag = readByte(BNO055_MAG_REV_ID_ADDR);
    int gyro = readByte(BNO055_GYRO_REV_ID_ADDR);
    int bl = readByte(BNO055_BL_REV_ID_ADDR);
    int sw_lsb = readByte(BNO055_SW_REV_ID_LSB_ADDR);
    int sw_msb = readByte(BNO055_SW_REV_ID_MSB_ADDR);
    int sw = ((sw_msb << 8) | sw_lsb) & 0xFFFF;
    std::array<int, 5> ret{sw, bl, accel, mag, gyro};
    return ret;
}

void BNO055::setExternalCrystal(bool external_crystal)
{
    configMode();
    int data = external_crystal ? 0x80 : 0x00;
    writeByte(BNO055_SYS_TRIGGER_ADDR, data);
    operationMode();
}

std::array<int, 3> BNO055::getSystemStatus(bool run_self_test)
{
    int self_test = -1;
    if (run_self_test)
    {
        configMode();
        int sys_trigger = readByte(BNO055_SYS_TRIGGER_ADDR);
        writeByte(BNO055_SYS_TRIGGER_ADDR, sys_trigger | 0x1);
        delay(1000);
        self_test = readByte(BNO055_SELFTEST_RESULT_ADDR);
        operationMode();
    }

    int status = readByte(BNO055_SYS_STAT_ADDR);
    int error = readByte(BNO055_SYS_STAT_ADDR);
    std::array<int, 3> ret{status, self_test, error};
    return ret;
}

std::array<int, 4> BNO055::getCalibrationStatus()
{
    int cal_status = readByte(BNO055_CALIB_STAT_ADDR);
    int sys = (cal_status >> 6) & 0x03;
    int gyro = (cal_status >> 4) & 0x03;
    int accel = (cal_status >> 2) & 0x03;
    int mag = cal_status & 0x03;
    std::array<int, 4> ret{sys, gyro, accel, mag};
    return ret;
}

std::array<int, 22> BNO055::getCalibration()
{
    configMode();
    std::array<int, 22> ret;
    std::vector<int> buf = readBytes(ACCEL_OFFSET_X_LSB_ADDR, 22);
    std::copy_n(buf.begin(), 22, ret.begin());
    operationMode();
    return ret;
}

void BNO055::setCalibration(std::array<int, 22> data)
{
    configMode();
    std::vector<int> buf(data.begin(), data.end());
    writeBytes(ACCEL_OFFSET_X_LSB_ADDR, buf);
    operationMode();
}

std::array<int, 6> BNO055::getAxisRemap()
{
    int map_config = readByte(BNO055_AXIS_MAP_CONFIG_ADDR);
    int z = (map_config >> 4) & 0x03;
    int y = (map_config >> 2) & 0x03;
    int x = map_config & 0x03;
    int sign_config = readByte(BNO055_AXIS_MAP_SIGN_ADDR);
    int x_sign = (sign_config >> 2) & 0x01;
    int y_sign = (sign_config >> 1) & 0x01;
    int z_sign = sign_config & 0x01;
    std::array<int, 6> ret{x, y, z, x_sign, y_sign, z_sign};
    return ret;
}

void BNO055::setAxisRemap(int x, int y, int z, int x_sign, int y_sign, int z_sign)
{
    configMode();
    int map_config = 0x00;
    map_config |= (z & 0x03) << 4;
    map_config |= (y & 0x03) << 2;
    map_config |= x & 0x03;
    writeByte(BNO055_AXIS_MAP_CONFIG_ADDR, map_config);
    int sign_config = 0x00;
    sign_config |= (x_sign & 0x01) << 2;
    sign_config |= (y_sign & 0x01) << 1;
    sign_config |= z_sign & 0x01;
    writeByte(BNO055_AXIS_MAP_SIGN_ADDR, sign_config);
    operationMode();
}

std::vector<int> BNO055::readVector(int reg, int count)
{
    std::vector<int> data = readBytes(reg, count);
    std::vector<int> ret;
    for (int i = 0; i < count; i++)
    {
        int val = ((data.at(i * 2 + 1) << 8) | data.at(i * 2)) & 0xFFFF;
        if (val > 32767)
        {
            val -= 65536;
        }

        ret.push_back(val);
    }

    return ret;
}

std::array<int, 3> BNO055::readEuler()
{
    std::vector<int> buf = readVector(BNO055_EULER_H_LSB_ADDR, 3);
    std::array<int, 3> ret;
    std::copy_n(buf.begin(), 3, ret.begin());
    return ret;
}

std::array<int, 3> BNO055::readMagnetometer()
{
    std::vector<int> buf = readVector(BNO055_MAG_DATA_X_LSB_ADDR, 3);
    std::array<int, 3> ret;
    std::copy_n(buf.begin(), 3, ret.begin());
    return ret;
}

std::array<int, 3> BNO055::readGyroscope()
{
    std::vector<int> buf = readVector(BNO055_GYRO_DATA_X_LSB_ADDR, 3);
    std::array<int, 3> ret;
    std::copy_n(buf.begin(), 3, ret.begin());
    return ret;
}

std::array<int, 3> BNO055::readAccelerometer()
{
    std::vector<int> buf = readVector(BNO055_ACCEL_DATA_X_LSB_ADDR, 3);
    std::array<int, 3> ret;
    std::copy_n(buf.begin(), 3, ret.begin());
    return ret;
}

std::array<int, 3> BNO055::readLinearAcceleration()
{
    std::vector<int> buf = readVector(BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR, 3);
    std::array<int, 3> ret;
    std::copy_n(buf.begin(), 3, ret.begin());
    return ret;
}

std::array<int, 3> BNO055::readGravity()
{
    std::vector<int> buf = readVector(BNO055_GRAVITY_DATA_X_LSB_ADDR, 3);
    std::array<int, 3> ret;
    std::copy_n(buf.begin(), 3, ret.begin());
    return ret;
}

std::array<int, 4> BNO055::readQuaternion()
{
    std::vector<int> buf = readVector(BNO055_QUATERNION_DATA_W_LSB_ADDR, 4);
    std::array<int, 4> ret;
    std::copy_n(buf.begin(), 4, ret.begin());
    return ret;
}

int BNO055::readTemp()
{
    return readSignedByte(BNO055_TEMP_ADDR);
}
