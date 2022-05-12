/*
* Interpretation functions
* Author: William Herse Locke Howard Jiayi Han
* Date: June 01, 2021

  The first is used to find the required change in angle when the gyro is being used to stabilise the device.
  The second is used to the find the required change in angle when the accelerometer is being used to stabilise the device.
  It involves finding the angle between the current acceleration vector and the desired vector, this angle is then returned to be used for correction, 
*/

#include <hidef.h>
#include "lidar.h"
#include "derivative.h"
#include <math.h>

//1 deg movement is 1000.
unsigned int anglemeasure(int gyro_y){
  unsigned int angle = 0;
  
  if (gyro_y > 0) {
    if ((gyro_y - (gyro_y/1000)*1000) > 500) {
     
       angle = gyro_y/1000 + 1;
      
    } else {
      
      angle = gyro_y/1000;
      
    }
  }
  if (gyro_y < 0) {
    
   if ((gyro_y + (gyro_y/1000)*1000) < -500) {
      
       angle = gyro_y/1000 - 1;
      
    } else {
      
      angle = gyro_y/1000;
      
    }
  }
    return angle;
 
}

int angleConversion(double rawX, double rawZ, double desiredX, double desiredZ) {
  int angleChange = 0; 
  //double currentVec[2]; 
  //double desiredVec[2];
  double sqrt_1,sqrt_2;
  double denom,num;
  double calc;
  //currentVec[0] = rawX*rawX;
  //currentVec[1] = rawZ*rawZ;
  //desiredVec[0] = desiredX*desiredX;
  //desiredVec[1] = desiredZ*desiredZ;
  sqrt_1 = sqrt((rawX*rawX)+(rawZ*rawZ));
  sqrt_2 = sqrt((desiredX*desiredX)+(desiredZ*desiredZ));
  denom = sqrt_1*sqrt_2;
  num = rawX*desiredX + rawZ*desiredZ;
  calc = num/denom;
  calc = acos(calc);
  calc = calc*180/3.1415926; 
  angleChange = (int)calc; 
  return angleChange;
  
}

