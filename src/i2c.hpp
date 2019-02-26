#ifndef H_I2C
#define H_I2C
#include <vector>
class I2C
{
  public:
	I2C(int);
	void init(int);
	int read();
	int write(int);
	int readByte(int);
	int writeByte(int, int);
	int readWord(int);
	int writeWord(int, int);
	int processCall(int, int);
	std::vector<int> readBlock(int, int);
	int writeBlock(int, std::vector<int>);
	static void delay(int);

  private:
	int fd;
};
#endif
