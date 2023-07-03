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
    static int sweep_right = 0;

    if (huskylens.available()){
      while (huskylens.available())
      {
          HUSKYLENSResult result = huskylens.read();
          printResult(result);
          if (result.command == COMMAND_RETURN_BLOCK){

            if(result.width > 60){
              continue;
            }

            if ((result.xCenter > 110) && (result.xCenter < 210)){

              robot.go(Direction::FORWARD, 150, 50);
            
            }  else if (result.xCenter >= 210 ) {
            
              robot.go(Direction::LEFT, 150, 50);
            
            } else if (result.xCenter <= 110) {
            
              robot.go(Direction::RIGHT, 150, 50);
            
            }
          }
      } 
    } else { 
                                                                                                                                                                                                             
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