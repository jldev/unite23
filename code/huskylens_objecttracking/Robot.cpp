/*
  Robot.cpp

  Helped class for controlling the robot.

*/
#include <Arduino.h>
#include "Robot.h"

Robot::Robot (uint8_t left_dir_pin, uint8_t left_pwm, uint8_t right_dir_pin, uint8_t right_pwm) {
  _left_dir_pin = left_dir_pin;
  _left_pwm = left_pwm;
  _right_dir_pin = right_dir_pin;
  _right_pwm = right_pwm;
  pinMode (left_dir_pin, OUTPUT);
  pinMode (_left_pwm, OUTPUT);
  pinMode (_right_dir_pin, OUTPUT);
  pinMode (_right_pwm, OUTPUT);
}


void Robot::go(Direction dir, uint8_t speed){
  switch(dir){
    case Direction::FORWARD:
      digitalWrite(_left_dir_pin, HIGH);
      digitalWrite(_right_dir_pin, LOW);
      break;
    case Direction::BACKWARD:
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, HIGH);
      break;
    case Direction::LEFT:
      digitalWrite(_left_dir_pin, HIGH);
      digitalWrite(_right_dir_pin, HIGH);
      break;
    case Direction::RIGHT:
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, LOW);
      break; 
    case Direction::STOP:
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, LOW);
    default:
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, LOW);
      break;     
  }
  analogWrite(_left_pwm, speed);
  analogWrite(_right_pwm, speed);
}

void Robot::stop(void){
  analogWrite(_left_pwm, 0);
  analogWrite(_right_pwm, 0);
}