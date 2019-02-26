#include <wiringPi.h>
#include "l298n.hpp"
#include "pca9685.hpp"
L298N::L298N(PCA9685 pca, int pin_enable, int pin_in1, int pin_in2) : pca(pca), pin_enable(pin_enable), pin_in1(pin_in1), pin_in2(pin_in2)
{
	wiringPiSetupPhys();
	pinMode(pin_in1, OUTPUT);
	pinMode(pin_in2, OUTPUT);
}

void L298N::setSpeed(int speed)
{
	int in1_val;
	int in2_val;
	if (0 < speed)
	{
		in1_val = HIGH;
		in2_val = LOW;
	}
	else if (speed < 0)
	{
		in1_val = LOW;
		in2_val = HIGH;
		speed = abs(speed);
	}
	else
	{
		in1_val = LOW;
		in2_val = LOW;
		speed = 4096;
	}
	digitalWrite(pin_in1, pin_in1);
	digitalWrite(pin_in2, pin_in2);
	pca.setPWM(pin_enable, speed);
}

void L298N::stop()
{
	setSpeed(0);
}
