#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <time.h>
#include "i2c-dev.h"

#include "i2c.hpp"

I2C::I2C(int file)
{
	std::string filename = "/dev/i2c-" + std::to_string(file);
	fd = open(filename.c_str(), O_RDWR);
	if (fd < 0)
	{
		std::cout << "Failed to open i2c interface.\n";
	}
}

void I2C::init(int addr)
{
	if (ioctl(fd, I2C_SLAVE, addr) < 0)
	{
		std::cout << "Failed to access slave.\n";
	}
}

int I2C::read()
{
	return i2c_smbus_read_byte(fd);
}

int I2C::write(int data)
{
	return i2c_smbus_write_byte(fd, data);
}

int I2C::readByte(int reg)
{
	return i2c_smbus_read_byte_data(fd, reg);
}

int I2C::writeByte(int reg, int data)
{
	return i2c_smbus_write_byte_data(fd, reg, data);
}

int I2C::readWord(int reg)
{
	return i2c_smbus_read_word_data(fd, reg);
}

int I2C::writeWord(int reg, int data)
{
	return i2c_smbus_write_word_data(fd, reg, data);
}

int I2C::processCall(int reg, int data)
{
	return i2c_smbus_process_call(fd, reg, data);
}

std::vector<int> I2C::readBlock(int reg, int length)
{
	__u8 data[length];
	i2c_smbus_read_i2c_block_data(fd, reg, length, data);
	std::vector<int> ret(data, data + length);
	return ret;
}

int I2C::writeBlock(int reg, std::vector<int> data)
{
	if (data.size() > 32)
	{
		std::cout << "Vector is greater than 32 bytes, only using the first 32.\n";
	}
	return i2c_smbus_write_i2c_block_data(fd, reg, data.size(), (__u8 *)data.data());
}

void I2C::delay(int how_long)
{
	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = how_long * 1000000L;
	nanosleep(&req, (struct timespec *)NULL);
}