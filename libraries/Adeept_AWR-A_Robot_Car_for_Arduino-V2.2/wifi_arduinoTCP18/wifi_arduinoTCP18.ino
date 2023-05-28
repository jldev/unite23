/***********************************************************
File name:  wifi_arduinoTCP18.ino
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2019/10/30 
***********************************************************/
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            8
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      3
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Servo ultrasonicServo;           // define servo to control turning of ultrasonic sensor
int trigPin = 10;                  // define Trig pin for ultrasonic ranging module
int echoPin = 11;                  // define Echo pin for ultrasonic ranging module
float maxDistance = 200;          // define the range(cm) for ultrasonic ranging module, Maximum sensor distance is rated at 400-500cm.
float soundVelocity = 340;        // Sound velocity = 340 m/s
float rangingTimeOut = 2 * maxDistance / 100 / soundVelocity * 1000000; // define the timeout(ms) for ultrasonic ranging module
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
char judge = 0;
bool processingID = false;
String comdata = "";
String   text1 = "\r\n+IPD,0,12:forwardStart";//w
//String  text11 = "\r\n+IPD,0,11:forwardStop";
String  phone1 = "\r\n+IPD,0,13:forwardStart\n";//w
//String phone11 = "\r\n+IPD,0,12:forwardStop\n";
String   text2 = "\r\n+IPD,0,9:leftStart";//a
//String  text21 = "\r\n+IPD,0,8:leftStop";
String  phone2 = "\r\n+IPD,0,10:leftStart\n";//a
//String phone21 = "\r\n+IPD,0,9:leftStop\n";
String   text3 = "\r\n+IPD,0,10:rightStart";//d
//String  text31 = "\r\n+IPD,0,9:rightStop";
String  phone3 = "\r\n+IPD,0,11:rightStart\n";//d
//String phone31 = "\r\n+IPD,0,10:rightStop\n";
String   text4 = "\r\n+IPD,0,13:backwardStart";//s
//String  text41 = "\r\n+IPD,0,12:backwardStop";
String  phone4 = "\r\n+IPD,0,14:backwardStart\n";//s
//String phone41 = "\r\n+IPD,0,13:backwardStop\n";

String text6 = "\r\n+IPD,0,1:6";
String text7 = "\r\n+IPD,0,1:7";
String text8 = "\r\n+IPD,0,1:8";
String text9 = "\r\n+IPD,0,1:9";

String   text1T = "\r\n+IPD,1,12:forwardStart";//w
//String  text11T = "\r\n+IPD,1,11:forwardStop";
String  phone1T = "\r\n+IPD,1,13:forwardStart\n";//w
//String phone11T = "\r\n+IPD,1,12:forwardStop\n";
String   text2T = "\r\n+IPD,1,9:leftStart";//a
//String  text21T = "\r\n+IPD,1,8:leftStop";
String  phone2T = "\r\n+IPD,1,10:leftStart\n";//a
//String phone21T = "\r\n+IPD,1,9:leftStop\n";
String   text3T = "\r\n+IPD,1,10:rightStart";//d
//String  text31T = "\r\n+IPD,1,9:rightStop";
String  phone3T = "\r\n+IPD,1,11:rightStart\n";//d
//String phone31T = "\r\n+IPD,1,10:rightStop\n";
String   text4T = "\r\n+IPD,1,13:backwardStart";//s
//String  text41T = "\r\n+IPD,1,12:backwardStop";
String  phone4T = "\r\n+IPD,1,14:backwardStart\n";//s
//String phone41T = "\r\n+IPD,1,13:backwardStop\n";

String  text5T = "\r\n+IPD,1,1:5";
String phone5T = "\r\n+IPD,1,2:5\n";
String text6T = "\r\n+IPD,1,1:6";
String text7T = "\r\n+IPD,1,1:7";
String text8T = "\r\n+IPD,1,1:8";
String text9T = "\r\n+IPD,1,1:9";

String text10 = "\r\nSEND OK";
int distance;
int yan = 0;
void setup() {
  // put your setup code here, to run once:
  ESP8266_ATCOMMAND();
  pinMode(dirAPin, OUTPUT);   // set dirAPin to output mode
  pinMode(pwmAPin, OUTPUT);   // set pwmAPin to output mode
  pinMode(dirBPin, OUTPUT);   // set dirBPin to output mode
  pinMode(pwmBPin, OUTPUT);   // set pwmBPin to output mode 
  ctrlCar(STOP,0);//A0, A1, A2 tracking does not detect black lines
  ultrasonicServo.attach(12);  // attaches the servo on ultrasonicPin to the servo object
  pinMode(trigPin, OUTPUT); // set trigPin to output mode
  pinMode(echoPin, INPUT);  // set echoPin to input mode
  ultrasonicServo.write(90);
  pinMode(13, OUTPUT);   // set dirAPin to output mode
  for(int i=0;i<3;i++){
      pixels.setPixelColor(i,pixels.Color(0,0,0));
      pixels.show();    
      delay(50);       
  } 
}
void loop() {

  while(Serial.available()>0)
  {  
    comdata += char(Serial.read());
    delay(1);
    
  }  
//  digitalWrite(13,HIGH);delay(100);digitalWrite(13,LOW);delay(100);
  judgement();
  if(judge==6){
    radarScanning();
    ultrasonicServo.write(90);
  }
  if(judge==7){
    Tracking();
  }
  if(judge==8){
    obstacleAvoidance(); 
    ultrasonicServo.write(90);
  }
}
void ESP8266_ATCOMMAND(){
   
   Serial.begin(115200);       // set up a wifi serial communication baud rate 115200
   Serial.println("AT+RST\r\n");     //reset wifi
   pixels.begin(); // This initializes the NeoPixel library.
   pixels.setPixelColor(0,pixels.Color(0,50,0));
   pixels.show();     
   delay(3000);     //delay 4s
   Serial.println("AT+CWMODE=3\r\n");//set to softAP+station mode
   delay(3000);     //delay 4s  
   Serial.println("AT+CIPMUX=1\r\n");
   pixels.begin(); // This initializes the NeoPixel library.
   pixels.setPixelColor(0,pixels.Color(0,50,0));
   pixels.show();    
   delay(50); 
   pixels.setPixelColor(1,pixels.Color(0,50,0));
   pixels.show();    
   delay(50); 
   delay(3000);
   Serial.println("AT+CIPSERVER=1\r\n");
   delay(3000);
   Serial.println("AT+CIPSTO=7000\r\n");
   pixels.begin(); // This initializes the NeoPixel library.
   pixels.setPixelColor(0,pixels.Color(0,50,0));
   pixels.show();    
   delay(50); 
   pixels.setPixelColor(1,pixels.Color(0,50,0));
   pixels.show();    
   delay(50); 
   pixels.setPixelColor(2,pixels.Color(0,50,0));
   pixels.show();     
   delay(3000);
}
//Control motor motion direction and speed function
void ctrlCar( int motorDir, byte motorSpd) {
  switch(motorDir){
    case 1:digitalWrite(dirAPin, HIGH);
           digitalWrite(dirBPin, LOW);
           break;
    case 2:digitalWrite(dirAPin, LOW);
           digitalWrite(dirBPin, HIGH);
           break;
    case 3:digitalWrite(dirAPin, HIGH);
           digitalWrite(dirBPin, HIGH);
           break;
    case 4:digitalWrite(dirAPin, LOW);
           digitalWrite(dirBPin, LOW);
           break;    
    default:digitalWrite(dirAPin, LOW);
           digitalWrite(dirBPin, LOW);
           break;     
  }
  analogWrite(pwmAPin, motorSpd);
  analogWrite(pwmBPin, motorSpd);
}
float getDistance() {
  unsigned long pingTime; // save the high level time returned by ultrasonic ranging module
  float distance;         // save the distance away from obstacle
  // set the trigPin output 10us high level to make the ultrasonic ranging module start to measure
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // get the high level time returned by ultrasonic ranging module
  pingTime = pulseIn(echoPin, HIGH, rangingTimeOut);
  if (pingTime != 0) {  // if the measure is not overtime
    distance = pingTime * soundVelocity / 2 / 10000;  // calculate the obstacle distance(cm) according to the time of high level returned
    return distance;    // return distance(cm)
  }
  else                  // if the measure is overtime
    return maxDistance; // returns the maximum distance(cm)
}
void judgement(){
    if(comdata.length() > 0)
    {    
        
        if(comdata==text1||comdata==text1T){judge=1;}
        else if(comdata==text2||comdata==text2T){judge=2;}
        else if(comdata==text3||comdata==text3T){judge=3;}
        else if(comdata==text4||comdata==text4T){judge=4;}
        else if(comdata==phone1||comdata==phone1T){judge=11;}
        else if(comdata==phone2||comdata==phone2T){judge=12;}
        else if(comdata==phone3||comdata==phone3T){judge=13;}
        else if(comdata==phone4||comdata==phone4T){judge=14;}
        else if(comdata==text6||comdata==text6T){judge=6;}
        else if(comdata==text7||comdata==text7T){judge=7;}
        else if(comdata==text8||comdata==text8T){judge=8;}
        else if(comdata==text10){judge=1;}
        else{judge=5;}

        if(comdata==text1T||comdata==text2T||comdata==text3T||comdata==text4T){
          processingID = true;
          }
       if(comdata==text5T||comdata==text6T||comdata==text7T||comdata==text8T){
          processingID = true;
          }
        comdata = "";
    }
    switch(judge)
    {
        case 1: ctrlCar(FORWARD,230);delay(100);judge=5;
                   break;
        case 2: ctrlCar(LEFT,230);delay(100);judge=5;
                   break;
        case 3: ctrlCar(RIGHT,230);delay(100);judge=5;
                   break;
        case 4: ctrlCar(BACKWARD,230);delay(100);judge=5;
                   break;      
        case 5: ctrlCar(STOP,0);delay(1);
                   break;    
        case 6: ctrlCar(STOP,0);delay(1);
                   break; 
        case 11: ctrlCar(FORWARD,230);delay(100);
                   break; 
        case 12: ctrlCar(LEFT,230);delay(100);
                   break; 
        case 13: ctrlCar(RIGHT,230);delay(100);
                   break; 
        case 14: ctrlCar(BACKWARD,230);delay(100);
                   break;         
        default:   break;
    }
}
void Tracking(){
    if(analogRead(A0)>1000&&analogRead(A1)>1000&&analogRead(A2)>1000){
      ctrlCar(FORWARD,120);//Three tracking detections are black lines
    }
    if(analogRead(A0)>1000&&analogRead(A1)<100&&analogRead(A2)<100){
      ctrlCar(RIGHT,120);//Right A2 tracking detection is black line
    }
    if(analogRead(A0)<100&&analogRead(A1)>1000&&analogRead(A2)<100){
      ctrlCar(FORWARD,120);//The middle side A1 tracking is detected as a black line
    }
    if(analogRead(A0)<100&&analogRead(A1)<100&&analogRead(A2)>1000){
      ctrlCar(LEFT,120);//Left A0 tracking detection is black line
    }
    if(analogRead(A0)<100&&analogRead(A1)<100&&analogRead(A2)<100){
      ctrlCar(STOP,0);//A0, A1, A2 tracking does not detect black lines
    }
}
void obstacleAvoidance(){
  if(getDistance()<20){
    ctrlCar(LEFT,200);//Left A0 tracking detection is black line
    delay(100);  
  }else{
    ctrlCar(FORWARD,2000);//Three tracking detections are black lines
  }
}
void radarScanning(){
    judge = 5;
    for(int i=0;i<5;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////5/////////////////////////
    ultrasonicServo.write(5);
    sendWifi(5);
    for(int i=6;i<15;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////15////////////////////////
    ultrasonicServo.write(15);
    sendWifi(15);
    for(int i=16;i<25;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////25//////////////////////////
    ultrasonicServo.write(25);
    sendWifi(25);
    for(int i=26;i<35;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////35//////////////////////
    ultrasonicServo.write(35);
    sendWifi(35);
    for(int i=36;i<45;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////45//////////////////////
    ultrasonicServo.write(45);
    sendWifi(45);
    for(int i=46;i<55;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////55//////////////////////
    ultrasonicServo.write(55);
    sendWifi(55);
    for(int i=56;i<65;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////65//////////////////////
    ultrasonicServo.write(65);
    sendWifi(65);
    for(int i=66;i<75;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////75//////////////////////
    ultrasonicServo.write(75);
    sendWifi(75);
    for(int i=76;i<85;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////85//////////////////////
    ultrasonicServo.write(85);
    sendWifi(85);
    for(int i=86;i<95;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////95//////////////////////
    ultrasonicServo.write(95);
    sendWifi(95);
    for(int i=96;i<105;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////105//////////////////////
    ultrasonicServo.write(105);
    sendWifi(105);
    for(int i=106;i<115;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////115//////////////////////
    ultrasonicServo.write(115);
    sendWifi(115);
    for(int i=116;i<125;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////125//////////////////////
    ultrasonicServo.write(125);
    sendWifi(125);
    for(int i=126;i<135;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////135//////////////////////
    ultrasonicServo.write(135);
    sendWifi(135);
    for(int i=136;i<145;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////145//////////////////////
    ultrasonicServo.write(145);
    sendWifi(145);
    for(int i=146;i<155;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////155//////////////////////
    ultrasonicServo.write(155);
    sendWifi(155);
    for(int i=156;i<165;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////165//////////////////////
    ultrasonicServo.write(165);
    sendWifi(165);
    for(int i=166;i<175;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
//////////////////////175//////////////////////
    ultrasonicServo.write(175);
    sendWifi(175);
    for(int i=176;i<180;i++){
      ultrasonicServo.write(i);
      delay(15);
    }
}
void sendWifi(int iAngle){
  distance = getDistance();
  if(distance<10||distance>40){distance=60;}
  if(processingID==true){
    Serial.println("AT+CIPSEND=1,8\r\n");
  }else{
    Serial.println("AT+CIPSEND=0,8\r\n");
  }
    delay(10);
    if(iAngle<10){
      Serial.print(0);
      Serial.print(0);
      Serial.print(iAngle); // Sends the current degree into the Serial Port
    }else if(iAngle<100){
      Serial.print(0);
      Serial.print(iAngle); // Sends the current degree into the Serial Port
    }else if(iAngle<180){
      Serial.print(iAngle); // Sends the current degree into the Serial Port
    }
    Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(0);
    Serial.print(distance); // Sends the distance value into the Serial Port
    Serial.println("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    delay(100);
}
