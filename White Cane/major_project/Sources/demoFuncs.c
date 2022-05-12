/*
* Servo demonstration functions
* Author: William Herse Locke Howard Jiayi Han
* Date: June 01, 2021

  These functions are used to pan the servo to show how movement. 
*/

#include <hidef.h>
#include "lidar.h"
#include "derivative.h"
#include <math.h>
#include "demoFuncs.h"
#include "servo.h"


void servoDemo(int i, int scalingFactor) {
  
  if (i % (8*scalingFactor) == 0) {
    
     angle1 = 0;
     angle2 = 90;
    
  } else if (i % (7*scalingFactor) == 0) {
    
     angle1 = 90;
     angle2 = 90;
     
  } else if (i % (6*scalingFactor) == 0) {
  
     angle1 = 180;
     angle2 = 90;
  
  } else if (i % (5*scalingFactor) == 0) {
  
     angle1 = 90;
     angle2 = 90;
  
  } else if (i % (4*scalingFactor) == 0) {
  
     angle1 = 90;
     angle2 = 0; 
  
  } else if (i % (3*scalingFactor) == 0) {
  
     angle1 = 90;
     angle2 = 90;
  
  } else if (i % (2*scalingFactor) == 0) {
  
     angle1 = 90;
     angle2 = 180;
  
  } else if (i % (scalingFactor) == 0){
  
     angle1 = 90;
     angle2 = 90;
  
  }
  
 
  
}

