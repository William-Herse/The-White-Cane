/*
* Main function
* Author: William Herse Locke Howard Jiayi Han
* Date: June 01, 2021

  The main execution function, handles module intergration. Allows for either individual demonstration or integrated execution.
*/




#include <hidef.h>      /* common defines and macros */
#include <assert.h>
#include "derivative.h"      /* derivative-specific definitions */

// need this for string functions
#include <stdio.h>

#include "buzzer.h"
#include "pll.h"
#include "simple_serial.h"
#include "l3g4200d.h"
#include "servo.h"
#include "rawinterp.h"
#include "demoFuncs.h"

//Flag used to demostrate each part of the product
// 0 - only the lidar readings
// 1 - servo control demonstration
// 2 - gyro based stabilisation
// 3 - accel based stabilation
// 4 - panning demo
// 5 - buzzer
// 6 - final product
int controlFlag = 6;

// global variable which denotes whether the buzzer should be on or off
extern int buzFlag = 0;

// global variable that defines the angle of the servo at any given time
extern int angle1 = 90;
extern int angle2 = 90;

// the reading returned from the lidar which is corelatted to the distance detected
long unsigned int distance = 0;

// variable that counts the loops through the endless for loop
int i = 1;

// buffer which stores the value output by the gyro over the amount of loops that have occured
int bufferG = 0;

// variable which stores how much the angle is required to change to maintain the desired absolute angle (using gyro)
int changeInAngle = 0;

// variable which stores how much the angle is required to change to maintain the desired absolute angle (using accel)
int anglechange = 0;

// variables which store the desired accelerometer angle which the ptu should maintain
double desiredX = 0.83;
double desiredZ = -0.33;

// flag which denotes which direction to adjust the angle by
int directionFlag = 1;

void main(void) {

  // create structs which store the raw and scaled values from the gyro, accel and magno
  AccelRaw read_accel;
  AccelScaled scaled_accel;
  GyroRaw read_gyro;
  MagRaw read_magnet;
  
  
  // var that stores the the error code 
  int error_code = NO_ERROR;
  
  // string for the buffer to be printed to serial
  unsigned char buffer[256];
  
  // used to write to the standard error file
  assert(error_code != NO_ERROR);

  #ifndef SIMULATION_TESTING

  // make sure the board is set to 24MHz
  PLL_Init();

  #endif


  // initialise the simple serial
  SCI1_Init(BAUD_9600);
  
  #ifndef SIMULATION_TESTING
  
  // initialise the sensor suite
  error_code = iicSensorInit();
  
  // write the result of the sensor initialisation to the serial
  if (error_code == NO_ERROR) {
    sprintf(buffer, "NO_ERROR");
    SCI1_OutString(buffer);
  } else {
    sprintf(buffer, "ERROR %d");
    SCI1_OutString(buffer);    
  }
  
  #else
  
  // intialise the clock registers for the TC7 to be used for the iic functions
  Init_TC7();
  
  // intitalise the clock registers for the buzzer
  Init_Buz();
  
  #endif

  //enable interrupts
	EnableInterrupts;
  COPCTL = 7;

  // set the servos to their starting position
  motor(dutycycle_conversion_tilt(angle2), dutycycle_conversion_pan(angle1));

  for(;;) {
  
    #ifndef SIMULATION_TESTING
  
    // read the raw values
    getRawDataGyro(&read_gyro);
    getRawDataAccel(&read_accel);
    getRawDataMagnet(&read_magnet);
    
    #else
    
    // inject some values for simulation
    read_gyro.x = 123; read_gyro.y = 313; read_gyro.z = 1002;
    read_accel.x = 124; read_accel.y = 312; read_accel.z = 2002;
    read_magnet.x = 125; read_magnet.y = 311; read_magnet.z = 3002;
    
    #endif

    // convert the acceleration to a scaled value
    convertUnits(&read_accel, &scaled_accel);    
   
    // calculate the difference between the current angle for the tilt and the desired angle
    anglechange = angleConversion(scaled_accel.x, scaled_accel.z, desiredX, desiredZ);
    
    // if the code is in the servo demo mode
    if (controlFlag == 1 && i <= 90) {
      
      // run the demo
      servoDemo(i, 10);
      motor(dutycycle_conversion_tilt(angle2), dutycycle_conversion_pan(angle1));
    }
    
    // initalise the timer overflow on the timer
    Init_TOF();
    
    // if the code is set to lidar demo or to final demo
    if (controlFlag == 0 || controlFlag == 6) {
      //read from lidar
      distance = lidarRun();
    }

    //if the code is set to gyro demo mode
    if (controlFlag == 2) {
      
      // add gyro values to the buffer if it is larger then what is deemed to be noise
      if (read_gyro.y > 1000 || read_gyro.y < -900) {
      
         bufferG += read_gyro.y;
    
      }
      
    }
  
    // format the string of the sensor data to go the the serial
    sprintf(buffer, "accel vec(%.2f, %.2f, %.2f), gyro(x:%d,y:%d,z:%d), magno(%d, %d, %d), Lidar(%lu) Angle1:%d Angle2:%d \r\n", scaled_accel.x, scaled_accel.y, scaled_accel.z, read_gyro.x, read_gyro.y, read_gyro.z, read_magnet.x, read_magnet.y, read_magnet.z, distance, angle1, angle2);
    
    // output the data to serial
    SCI1_OutString(buffer);
    
    // if the code is set to lidar demo or to final demo
    if (controlFlag == 0 || controlFlag == 6){
      
      // if distance is greater then calibrated range for no obstacles or declines
      if (distance > 691) {
      
       //enable buzzer
       buzFlag = 1;
      
      // if distance is less then calibrated range for no obstacles or declines 
      } else if (distance < 421) {
      
       //enable buzzer
       buzFlag = 1;
      
      } else {
      
      // disable buzzer
        buzFlag = 0;
      
      }
   }
   
   // if code is set to buzzer demo
   if (controlFlag == 5) {
   
      // enable buzzer
      buzFlag = 1;
   
   }
   
   //initalise buzzer timer registers
   Init_Buz();
   
   //enable buzzer
   buzFlag = Buz_Enable(buzFlag);

   // if code is set to gyro demonstration
   if (controlFlag == 2) {
      
      // every four loops
      if(i % 4 == 0) {
       
         // adjust the angles by the amount the gyro says that the angle has moved
         motor(dutycycle_conversion_tilt(angle2 - anglemeasure(bufferG) - changeInAngle), dutycycle_conversion_pan(90));
         changeInAngle = anglemeasure(bufferG);
         angle2 = angle2-anglemeasure(bufferG);
         bufferG = 0;
        
      } else{}
     
     }
    
    // if code is set to panning demo mode
    if (controlFlag == 4 || controlFlag == 6) {
    
        // if angle has reached the lower limit of pannig
        if (angle1 < 70) {
            
            // pan upwards
            directionFlag = 1;
        
        // if angle has reached the upper limit of panning    
        } else if (angle1 > 110) {
            
            // pan downwards
            directionFlag = -1;

        }
      
      //actuate panning 
      angle1 = angle1 + (directionFlag*5);
      motor(dutycycle_conversion_tilt(angle2), dutycycle_conversion_pan(angle1));
  
     
    }
    
    // if code is in accel demo mode
    if (controlFlag == 3 || controlFlag == 6) {
      
      // every 4 loops
      if(i % 4 == 0) {
      
        //if it needs to move upwards
        if (scaled_accel.z < desiredZ) {
          
            motor(dutycycle_conversion_tilt(angle2 + anglechange), dutycycle_conversion_pan(angle1));

            angle2 = (angle2 + anglechange);
        
        //if it needs to move downwards    
        } else if (scaled_accel.z > desiredZ) {
          
            motor(dutycycle_conversion_tilt(angle2 - anglechange), dutycycle_conversion_pan(angle1));

            angle2 = (angle2 - anglechange);
          
        }
        
   }
      
    }
    
    // increment counter for loops
    i++;
    
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  
  /* please make sure that you never leave main */
}
