/*
  Robot.h

  Helped class for controlling the robot.

*/

#include <Arduino.h>

enum class Direction {
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
    STOP
};

class Robot {
private:
  bool _left_advance;// = LOW;
  bool _left_back;// = HIGH;
  bool _right_advance;// = HIGH;
  bool _right_back;// = LOW;

  uint8_t _left_dir_pin;
  uint8_t _left_pwm;
  uint8_t _right_dir_pin;
  uint8_t _right_pwm;

public:
  Robot (uint8_t left_dir_pin, uint8_t left_pwm, uint8_t right_dir_pin, uint8_t right_pwm);
  void go(Direction dir, uint8_t speed);
  void go_smooth(Direction dir, uint8_t left_speed, uint8_t right_speed);
  void set_direction(Direction dir);
  void stop();

};


