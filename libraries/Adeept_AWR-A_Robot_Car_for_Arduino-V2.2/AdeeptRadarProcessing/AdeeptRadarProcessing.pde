/******************************************************************************
File name:   AdeeptRadarProcessing.pde
Description: Wireless radar scanning mode: the specific operation of this mode 
             is to connect the remote control to the computer, and run Processing 
             program we provide (AdeeptRadarProcessing. Pde). Press button D on 
             the remote control, and the LED1 on the remote control flashes, LED2 
             goes out and LED3 goes out. The four-wheel car is now in wireless 
             radar scanning mode. Data from distant car can be seen on a computer 
             screen and displayed as a real-time radar scan.
Website: www.adeept.com
E-mail: support@adeept.com
Author: Tom
Date: 2019/08/03 
********************************************************************************/ 

import controlP5.*;
import processing.net.*;
import java.awt.event.KeyEvent; // imports library for reading the data from the serial port
import java.io.IOException;

ControlP5 cp5;
controlP5.Button b1;
controlP5.Button b2;
controlP5.Button m1;
controlP5.Button m2;
controlP5.Button m3;
controlP5.Button m4;
controlP5.Button m5;
controlP5.Button m6;
controlP5.Button m7;
controlP5.Button m8;
controlP5.Button m9;
controlP5.Button m10;
controlP5.Button k1;
controlP5.Button k2;
controlP5.Button k3;
controlP5.Button k4;
controlP5.Button k5;
controlP5.Button k6;
controlP5.Button k7;
controlP5.Button k8;
controlP5.Button k9;
controlP5.Button k10;

Textlabel label1;
Textlabel label2;
Textlabel label3;
Textlabel label4;
Textlabel label5;
Textlabel label6;
Textlabel label7;
Textlabel label8;

int buttonValue = 1;

char letter;

boolean isOpen=true;

boolean b1Open,b2Open,Aopen,Sopen,btMute;

boolean toggleValue = false;

float b1x=-300,b1y=190,b2x=-300,b2y=190,m1x=-200,m1y=330,m2x=1400,m2y=330,m3x=-200,m3y=390;
float m4x=1400,m4y=390,m5x=-200,m5y=450,m6x=1400,m6y=450,m7x=-200,m7y=510,m8x=1400,m8y=510,m9x=1400,m9y=570;
float m10x=1400,m10y=570,k1x=-200,k1y=430,k2x=1400,k2y=430,k3x=-200,k3y=430,k4x=1400,k4y=430,k5x=-200,k5y=430;
float k6x=1400,k6y=430,k7x=-200,k7y=430,k8x=1400,k8y=430,k9x=1400,k9y=430,k10x=1400,k10y=430;
float label1x=-600,label1y=600,label2x=-600,label2y=600,label3x=-600,label3y=600,label4x=-600,label4y=380;
float label5x=-600,label5y=380,label6x=1300,label6y=380,label7x=1300,label7y=380,label8x=1300,label8y=380;



// defubes variables
String angle="";
String distance="";
String data="";
String noObject;
float pixsDistance;
int iAngle=181, iDistance, iOldAngle;
int index1=0;
int index2=0;
PFont orcFont;

Client myClient; 
int dataIn;
int dataIn0;
int dataIn1;
int dataIn2;
int dataIn3;
int dataIn4;
int dataIn5;
int dataIn6;
int dataIn7;
int dataNum=0;
byte[] byteBuffer = new byte[12];
byte interesting = 8;
String inString;

void setup() {
  
 size (1200, 900); // ***CHANGE THIS TO YOUR SCREEN RESOLUTION***(1200, 700);
 smooth();
 myClient = new Client(this, "192.168.4.1", 333); 
   
  cp5 = new ControlP5(this);
  
  k1 = cp5.addButton("q")//q
  .setValue(4)
  .setPosition(-200,430)
  .setSize(90,60);

  k2 = cp5.addButton("w")//w
  .setValue(4)
  .setPosition(1400,430)
  .setSize(90,60);
        
  k3 = cp5.addButton("e")//e
  .setValue(4)
  .setPosition(-200,430)
  .setSize(90,60);
        
  k4 = cp5.addButton("r")//r
  .setValue(4)
  .setPosition(1400,430)
  .setSize(90,60);
        
  k5 = cp5.addButton("t")//t
  .setValue(4)
  .setPosition(-200,430)
  .setSize(150,60);

  k6 = cp5.addButton("y")//y
  .setValue(4)
  .setPosition(1400,430)
  .setSize(150,60);

  k7 = cp5.addButton("u")//u
  .setValue(4)
  .setPosition(-200,430)
  .setSize(150,60);
               
  PFont pfont = createFont("Arial",20,true);
  ControlFont font = new ControlFont(pfont,241);
    
  k1.getCaptionLabel()
  .setFont(font)
  .setSize(15)
  .toUpperCase(false)
  .setText("forward");//A
  
  k2.getCaptionLabel()
  .setFont(font)
  .setSize(15)
  .toUpperCase(false)
  .setText("Turn left");//S
  
  k3.getCaptionLabel()
  .setFont(font)
  .setSize(15)
  .toUpperCase(false)
  .setText("Turn right");//D
  
  k4.getCaptionLabel()
  .setFont(font)
  .setSize(15)
  .toUpperCase(false)
  .setText("Back");//F
  
  k5.getCaptionLabel()
  .setFont(font)
  .setSize(15)
  .toUpperCase(false)
  .setText("Radar scanning");//G
  
  k6.getCaptionLabel()
  .setFont(font)
  .setSize(15)
  .toUpperCase(false)
  .setText("Tracking function");//H

  k7.getCaptionLabel()
  .setFont(font)
  .setSize(15)
  .toUpperCase(false)
  .setText("Obstacle avoidance");//J
}
void draw() {
  fill(98,245,31);
  // simulating motion blur and slow fade of the moving line
  noStroke();
  fill(0,4); 
  rect(0, 0, width, height-height*0.065); 
  
  fill(98,245,31); // green color
  //// calls the functions for drawing the radar
  drawRadar(); 
  drawLine();
  drawObject();
  drawText();

  // Receive data from server
  if (myClient.available() > 0) {
    if(dataNum>=8){dataNum=0;}
    if(dataNum==0){dataIn0 = myClient.read();}
    if(dataNum==1){dataIn1 = myClient.read();}
    if(dataNum==2){dataIn2 = myClient.read();}
    if(dataNum==3){dataIn3 = myClient.read();}
    if(dataNum==4){dataIn4 = myClient.read();}
    if(dataNum==5){dataIn5 = myClient.read();}
    if(dataNum==6){dataIn6 = myClient.read();}
    if(dataNum==7){dataIn7 = myClient.read();}
    if(dataNum==7&&dataIn7==46){
      iAngle = (dataIn0-48)*100+(dataIn1-48)*10+dataIn2-48;
      iDistance = (dataIn4-48)*100+(dataIn5-48)*10+dataIn6-48;
      iOldAngle = iAngle;
      iAngle = iAngle-5;
    }
     dataNum++;
     //print(iAngle);
     //print("   ");
     //println(iDistance);
     delay(10);
  }
  if(iAngle<(iOldAngle+10)){
    iAngle++;
    //delay(5);
  }
  
 textSize(70);
 fill(169,169,169);
 text("AWR-A Robot GUI",300,100);
    k1.setPosition(110,130);
    k2.setPosition(10,200);
    k3.setPosition(210,200);
    k4.setPosition(110,270);
    k5.setPosition(1030,140);
    k6.setPosition(1030,220);
    k7.setPosition(1030,300);
    buttonColor();
    mouseCheck();  
}
void drawRadar() {
  pushMatrix();
  translate(width/2,height-height*0.074); // moves the starting coordinats to new location
  noFill();
  strokeWeight(2);
  stroke(98,245,31);
  // draws the arc lines
  arc(0,0,(width-width*0.0625),(width-width*0.0625),PI,TWO_PI);
  arc(0,0,(width-width*0.27),(width-width*0.27),PI,TWO_PI);
  arc(0,0,(width-width*0.479),(width-width*0.479),PI,TWO_PI);
  arc(0,0,(width-width*0.687),(width-width*0.687),PI,TWO_PI);
  // draws the angle lines
  line(-width/2,0,width/2,0);
  line(0,0,(-width/2)*cos(radians(30)),(-width/2)*sin(radians(30)));
  line(0,0,(-width/2)*cos(radians(60)),(-width/2)*sin(radians(60)));
  line(0,0,(-width/2)*cos(radians(90)),(-width/2)*sin(radians(90)));
  line(0,0,(-width/2)*cos(radians(120)),(-width/2)*sin(radians(120)));
  line(0,0,(-width/2)*cos(radians(150)),(-width/2)*sin(radians(150)));
  line((-width/2)*cos(radians(30)),0,width/2,0);
  popMatrix();
}
void drawObject() {
  pushMatrix();
  translate(width/2,height-height*0.074); // moves the starting coordinats to new location
  strokeWeight(9);
  stroke(255,10,10); // red color
  pixsDistance = iDistance*((height-height*0.1666)*0.025); // covers the distance from the sensor from cm to pixels
  // limiting the range to 40 cms
  if(iDistance<40){
    // draws the object according to the angle and the distance
  line(pixsDistance*cos(radians(iAngle)),-pixsDistance*sin(radians(iAngle)),(width-width*0.505)*cos(radians(iAngle)),-(width-width*0.505)*sin(radians(iAngle)));
  }
  popMatrix();
}
void drawLine() {
  pushMatrix();
  strokeWeight(9);
  stroke(30,250,60);
  translate(width/2,height-height*0.074); // moves the starting coordinats to new location
  line(0,0,(height-height*0.3)*cos(radians(iAngle)),-(height-height*0.3)*sin(radians(iAngle))); // draws the line according to the angle
  popMatrix();//0.12
}
void drawText() { // draws the texts on the screen
  
  pushMatrix();
  if(iDistance>40) {
  noObject = "Out of Range";
  }
  else {
  noObject = "In Range";
  }
  fill(0,0,0);
  noStroke();
  rect(0, height-height*0.0648, width, height);
  fill(98,245,31);
  textSize(25);
  
  text("10cm",width-width*0.3854,height-height*0.0833);
  text("20cm",width-width*0.281,height-height*0.0833);
  text("30cm",width-width*0.177,height-height*0.0833);
  text("40cm",width-width*0.0729,height-height*0.0833);
  textSize(40);
  text("www.adeept.com ", width-width*0.875, height-height*0.0277);
  text("Angle: " + iAngle +" °", width-width*0.48, height-height*0.0277);
  text("Distance:", width-width*0.26, height-height*0.0277);
  if(iDistance<40) {
  text("          " + iDistance +"cm", width-width*0.215, height-height*0.0277);
  }
  textSize(25);
  fill(98,245,60);
  translate((width-width*0.4994)+width/2*cos(radians(30)),(height-height*0.0907)-width/2*sin(radians(30)));
  rotate(-radians(-60));
  text("30°",0,0);
  resetMatrix();
  translate((width-width*0.503)+width/2*cos(radians(60)),(height-height*0.0888)-width/2*sin(radians(60)));
  rotate(-radians(-30));
  text("60°",0,0);
  resetMatrix();
  translate((width-width*0.507)+width/2*cos(radians(90)),(height-height*0.0833)-width/2*sin(radians(90)));
  rotate(radians(0));
  text("90°",0,0);
  resetMatrix();
  translate(width-width*0.513+width/2*cos(radians(120)),(height-height*0.07129)-width/2*sin(radians(120)));
  rotate(radians(-30));
  text("120°",0,0);
  resetMatrix();
  translate((width-width*0.5104)+width/2*cos(radians(150)),(height-height*0.0574)-width/2*sin(radians(150)));
  rotate(radians(-60));
  text("150°",0,0);
  popMatrix(); 
}
void keyPressed(){
          if (key >= 'A' && key <= 'z') {
             //port1.write(key);
             //println(key);
           } 
}
void mouseCheck(){
           if(k1.isPressed()){
             myClient.write("forwardStart");
             delay(90);
           }
           if(k2.isPressed()){
             myClient.write("leftStart");
             delay(90);
           }
           if(k3.isPressed()){
             myClient.write("rightStart");
             delay(90);
           }
           if(k4.isPressed()){
             myClient.write("backwardStart");
             delay(90); 
           }         
           if(k5.isPressed()){
             //k5.setColorBackground(color(255,0,0));
             myClient.write("6");
             delay(10);
           } 
           if(k6.isPressed()){
             //port1.write("y");   
             myClient.write("7");
             delay(50);
           }
           if(k7.isPressed()){
             //port1.write("u");   
             myClient.write("8");
             delay(50);
           }
}
void buttonColor(){
       if(keyPressed && key=='w'){//w
          k1.setColorBackground(color(255,0,0));
          myClient.write("forwardStart");
          delay(100);
       }
       else{
          k1.setColorBackground(color(2,72,90));
          //myClient.write("w");
       }
       if(keyPressed && key=='a'){//a
          k2.setColorBackground(color(255,0,0));
          myClient.write("leftStart");
          delay(100);
       }
       else{
          k2.setColorBackground(color(2,72,90)); 
       }
       if(keyPressed && key=='d'){//d
          k3.setColorBackground(color(255,0,0));
          myClient.write("rightStart");
          delay(100);
       }
        else{
          k3.setColorBackground(color(2,72,90));
       }
       if(keyPressed && key=='s'){//s
          k4.setColorBackground(color(255,0,0));
          myClient.write("backwardStart");
          delay(100);
       }
        else{
          k4.setColorBackground(color(2,72,90));
       }
       if(keyPressed && key=='t'){//g
          k5.setColorBackground(color(255,0,0));
          myClient.write("6");
       }
        else{
          k5.setColorBackground(color(2,72,90));
       }
       k6.setColorBackground(color(2,72,90));
       k7.setColorBackground(color(2,72,90));
}
