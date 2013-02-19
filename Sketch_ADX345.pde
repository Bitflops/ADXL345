//Writen by Lapatate FEB 2013 for processing 2.0
//Reads ADXL345 acceleration sensor Data from Arduino serial output and displays acceleration graphs 

//This program is free software: you can redistribute it and/or modify
//it under the terms of the version 3 GNU General Public License as
//published by the Free Software Foundation.

//Put together by Bitflops on Feb-2013

import processing.serial.*;
import eeml.*;
import cc.arduino.*;

Serial arduino;

PFont myFont;  
PFont mySmallFont;// The display font: 

int windSizex = 400;
int windSizey = 800;

String data = null;
String[] elements = null;
String[] sensorValues = new String[15];

int lf = 13;

float[] valsx;
float[] valsy;
float[] valsz;
String range;
float accx = 0;
float accy = 0;
float accz = 0;
float accxmax = 0;
float accymax = 0;
float acczmax = 0;
float accxmin = 0;
float accymin = 0;
float acczmin = 0;

void setup() {
  //frameRate(256);
  size(windSizex, windSizey); 
  myFont = loadFont("Monospaced.plain-12.vlw");
  mySmallFont = loadFont("Arial-BoldMT-10.vlw");
  textFont(myFont);
  smooth();
  println(Arduino.list());
  arduino = new Serial(this, Serial.list()[0], 9600);
  arduino.bufferUntil(lf);
  
  valsx = new float[width-60];
  for (int i = 0; i < valsx.length; i++){
    valsx[i]= windSizey/2-256 ;
  }
  valsy = new float[width-60];
  for (int i = 0; i < valsy.length; i++){
    valsy[i]= windSizey/2;
  }
  valsz = new float[width-60];
  for (int i = 0; i < valsz.length; i++){
    valsz[i]= windSizey/2+256;
  }
}

void draw() {
   background(240,255,240);
   textFont(myFont);
 //Show Range 
   textAlign(CENTER);
   fill(0,0,0);
   text("Sensor Range: "+range,width/2,10);
   textAlign(LEFT);  
   text(frameRate,5,10);
  
 //Draw axis
   stroke(0,0,0);
   fill(0,0,0);
   textAlign(LEFT);
   line(0,height/2-256,width-20,height/2-256);
   line(width-60,height/2-320,width-60,height/2-190);
   text("X",width-10,height/2-252);
   line(0,height/2,width-20,height/2);
   line(width-60,height/2+70,width-60,height/2-70);
   text("Y",width-10,height/2);
   line(0,height/2+256,width-20,height/2+256);
   line(width-60,height/2+320,width-60,height/2+190);
   text("Z",width-10,height/2+260);
   
   stroke(0,0,0,30);
   line(0,height/2+53,width-30,height/2+53);
   text("+1G",width-25,height/2-49);
   text("-1G",width-25,height/2+57);
   line(0,height/2-53,width-30,height/2-53);
   line(0,height/2-256+53,width-30,height/2-256+53);
   text("+1G",width-25,height/2-256-49);
   text("-1G",width-25,height/2-256+57);
   line(0,height/2-256-53,width-30,height/2-256-53);
   line(0,height/2+256+53,width-30,height/2+256+53);
   text("+1G",width-25,height/2+256-49);
   text("-1G",width-25,height/2+256+57);
   line(0,height/2+256-53,width-30,height/2+256-53);
   
   textAlign(RIGHT);
   textFont(mySmallFont);
   if (accx >= accxmax){
     accxmax = accx;
     }
     if (accx <= accxmin){
     accxmin = accx;
     }
   fill(0,0,255,100);
   stroke(0,0,255);
   text("max "+(int(accxmax*100)/100.00),width-60,height/2-258-53*accxmax);
   line(0,height/2-256-53*accxmax,width-60,height/2-256-53*accxmax);
   text("min "+(int(accxmin*100)/100.00),width-60,height/2-247-53*accxmin);
   line(0,height/2-256-53*accxmin,width-60,height/2-256-53*accxmin);
   if (accy >= accymax){
     accymax = accy;
     }
     if (accy <= accymin){
     accymin = accy;
     }
   fill(0,200,0,100);
   stroke(0,200,0);
   text("max "+(int(accymax*100)/100.00),width-60,height/2-2-53*accymax);
   line(0,height/2-53*accymax,width-60,height/2-53*accymax);
   text("min "+(int(accymin*100)/100.00),width-60,height/2+9-53*accymin);
   line(0,height/2-53*accymin,width-60,height/2-53*accymin);
   if (accz >= acczmax){
     acczmax = accz;
     }
     if (accz <= acczmin){
     acczmin = accz;
     }
   fill(255,0,0,100);
   stroke(255,0,0,50);
   text("max "+(int(acczmax*100)/100.00),width-60,height/2+254-53*acczmax);
   line(0,height/2+256-53*acczmax,width-60,height/2+256-53*acczmax);
   text("min "+(int(acczmin*100)/100.00),width-60,height/2+265-53*acczmin);
   line(0,height/2+256-53*acczmin,width-60,height/2+256-53*acczmin);
   
 // Draw lines connecting all points
  for (int i = 0; i < valsx.length-1; i++) {
    stroke(0,0,255);
    strokeWeight(1);
    line(i,valsx[i],i+1,valsx[i+1]);
  }
  for (int i = 0; i < valsy.length-1; i++) {
    stroke(0,200,0);
    strokeWeight(1);
    line(i,valsy[i],i+1,valsy[i+1]);
  }
  for (int i = 0; i < valsz.length-1; i++) {
    stroke(255,0,0);
    strokeWeight(1);
    line(i,valsz[i],i+1,valsz[i+1]);
  } 
  
   // Slide everything down in the array
    for (int i = 0; i < valsx.length-1; i++) {
      valsx[i] = valsx[i+1]; 
    }
    // Add a new value
    valsx[valsx.length-1] = windSizey/2-256-53*accx;
    
    for (int i = 0; i < valsy.length-1; i++) {
      valsy[i] = valsy[i+1]; 
    }
    // Add a new value
    valsy[valsy.length-1] = windSizey/2-53*accy;
    
    for (int i = 0; i < valsz.length-1; i++) {
      valsz[i] = valsz[i+1]; 
    }
    // Add a new value
    valsz[valsz.length-1] = windSizey/2+256-53*accz;
    
    //Draw current value
    textAlign(LEFT);
    fill(0,0,255);
    text(int(accx*100)/100.00,width-60,windSizey/2-256-53*accx);
    fill(0,200,0);
    text(int(accy*100)/100.00,width-60,windSizey/2-53*accy);
    fill(255,0,0);
    text(int(accz*100)/100.00,width-60,windSizey/2+256-53*accz);
}   
    
void serialEvent(Serial p) {
  data = p.readString();
  // println(data);
  // just if there is data
  if (data != null) {
    if (data.charAt(1) == 'D') {
      // println("Data is beeing processed");
      // try catch function because of possible garbage in received data
      try {
        elements = splitTokens(data, ", ");
        print("Elements found: " + elements.length);
      // loop through sensor values 
      }
      catch (Exception e) {
        println("Dammit...something whent wrong");
      }
      for (int i = 0; i < elements.length ; i++ ) {
        print(elements[i]+"/");
        sensorValues[i] = elements[i];
      }     
     accx = float(sensorValues[2]);
     accy = float(sensorValues[4]);
     accz = float(sensorValues[6]);
     range = sensorValues[8];
     }  
   }
}
    
    
