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
  pinMode (_left_dir_pin, OUTPUT);
  pinMode (_left_pwm, OUTPUT);
  pinMode (_right_dir_pin, OUTPUT);
  pinMode (_right_pwm, OUTPUT);
}

void Robot::set_direction(Direction dir){
  
  switch(dir){
    case Direction::FORWARD:
      Serial.println(F("Set Direction = F"));
      digitalWrite(_left_dir_pin, HIGH);
      digitalWrite(_right_dir_pin, LOW);
      break;
    case Direction::BACKWARD:
      Serial.println(F("Set Direction = B"));
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, HIGH);
      break;
    case Direction::LEFT:
      Serial.println(F("Set Direction = L"));
      digitalWrite(_left_dir_pin, HIGH);
      digitalWrite(_right_dir_pin, HIGH);
      break;
    case Direction::RIGHT:
      Serial.println(F("Set Direction = R"));
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, LOW);
      break; 
    case Direction::STOP:
      Serial.println(F("Set Direction = S"));
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, LOW);
    default:
      digitalWrite(_left_dir_pin, LOW);
      digitalWrite(_right_dir_pin, LOW);
      break;     
  }
}

void Robot::go_smooth(Direction dir, uint8_t left_speed, uint8_t right_speed){
  set_direction(dir);
  analogWrite(_left_pwm, left_speed);
  analogWrite(_right_pwm, right_speed);
}

void Robot::go(Direction dir, uint8_t speed){
  go_smooth(dir, speed, speed);
}

void Robot::stop(void){
  analogWrite(_left_pwm, 0);
  analogWrite(_right_pwm, 0);
}