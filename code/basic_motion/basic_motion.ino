#include "Arduino.h"

//Motor control port
const int dirAPin = 7;    // define pin used to control rotational direction of motor A
const int pwmAPin = 6;    // define pin for PWM used to control rotational speed of motor A
const int dirBPin = 4;    // define pin used to control rotational direction of motor B
const int pwmBPin = 5;    // define pin for PWM used to control rotational speed of motor B
//Define motor control direction
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define STOP 4

void setup() {
  // put your setup code here, to run once:
  pinMode(dirAPin, OUTPUT);   // set dirAPin to output mode
  pinMode(pwmAPin, OUTPUT);   // set pwmAPin to output mode
  pinMode(dirBPin, OUTPUT);   // set dirBPin to output mode
  pinMode(pwmBPin, OUTPUT);   // set pwmBPin to output mode 
  ctrlCar(STOP,0);
  Serial.begin(9600);
}

void loop() {
  
  //Students will add code here to make the robot move 

  // example 
  // ctrlCar(LEFT, 150);
  // delay(1000);

}


//Control motor motion direction and speed function
void ctrlCar( int motorDir, byte motorSpd) {
  switch(motorDir){
    case FORWARD:
      digitalWrite(dirAPin, HIGH);
      digitalWrite(dirBPin, LOW);
      break;
    case BACKWARD:
      digitalWrite(dirAPin, LOW);
      digitalWrite(dirBPin, HIGH);
      break;
    case LEFT:
      digitalWrite(dirAPin, HIGH);
      digitalWrite(dirBPin, HIGH);
      break;
    case RIGHT:
      digitalWrite(dirAPin, LOW);
      digitalWrite(dirBPin, LOW);
      break; 
    case STOP:
      motorSpd = 0;
      digitalWrite(dirAPin, LOW);
      digitalWrite(dirBPin, LOW);
    default:
      digitalWrite(dirAPin, LOW);
      digitalWrite(dirBPin, LOW);
      break;     
  }
  analogWrite(pwmAPin, motorSpd);
  analogWrite(pwmBPin, motorSpd);
}