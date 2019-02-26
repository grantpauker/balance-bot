#ifndef H_L298N
#define H_L298N

#include "pca9685.hpp"

class L298N
{
public:
	L298N(PCA9685, int, int, int);
	void setSpeed(int);
	void stop();

private:
	int pin_enable;
	int pin_in1;
	int pin_in2;
	PCA9685 pca;
};
#endif
