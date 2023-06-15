#include "Arduino.h"
#include "HUSKYLENS.h"
#include "Robot.h"

HUSKYLENS huskylens;
Robot robot(7,6,4,5);

void printResult(HUSKYLENSResult result);

void setup() {
  robot.stop();
  Serial.begin(115200);
  Wire.begin();
  while (!huskylens.begin(Wire))
  {
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
  }
}

void loop() {
    int speed = 0;

    if (huskylens.available()){
      while (huskylens.available())
      {
          HUSKYLENSResult result = huskylens.read();
          // printResult(result);
          if (result.command == COMMAND_RETURN_BLOCK){
              //our speed will be based on the size of the object, smaller i.e. further away we go faster
              if (result.width > 100) { //we are pretty close
                speed = 0;
              } else if (result.width > 50){
                speed = 150;
              } else {
                speed = 200;
              }
              //x will go from 0 far left to 320 far right
              // let's segment the screen into 3 parts
              // 0 - 80 we need to turn right
              // 80 - 240 go straight
              // 240 - 320 we need to turn left
              if (result.xCenter < 80){
                robot.go(Direction::RIGHT, speed);
              } else if(result.xCenter >= 80 && result.xCenter <=240){
                robot.go(Direction::FORWARD, speed);
              } else {
                robot.go(Direction::LEFT, speed);
              }
          }
      } 
    } else {
      robot.stop();
      if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
      else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
      else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}