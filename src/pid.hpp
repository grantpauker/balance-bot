#ifndef H_PID
#define H_PID
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define BOUND(x, min, max) MIN(MAX(x, min), max)

class PID
{
public:
  float kp;
  float ki;
  float kd;
  float setpoint;
  float integral;
  float pre_error;
  float min_output;
  float max_output;

  PID(float, float, float);
  PID(float, float, float, float, float);

  float calculate(float, float, float);
  void reset();
};
#endif