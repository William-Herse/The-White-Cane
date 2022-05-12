/*
* Servo motor functions
* Author: William Herse Locke Howard Jiayi Han
* Date: June 01, 2021

The servo,c file aims to get the servo motor run from PWM signal. The duty cycle
of on over the whole period will decide the angle the servo is going to move to.
the motor's running frequeny is 50hz, while the 16 Bit timer inside the board is 24MHZ. 
Thus it needs to generate a long enough signal to match the timer frequency. 
16*3000 is used to fill the difference. The PWM signal length is timed by 3000. Prescale is 16.
The port used are port 5 and port 7. SO we need to use a 16bit register for PWM signal
to concatenate two ports. which is 45 and 67. 
Then control the pwmduty register the desired duty cycle.
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servo.h"



double angle_measured(volatile float duty_cycle){
  float angle = 0;
  if(duty_cycle*100>=4.5){
    
  angle = (duty_cycle*100 - 4.5)*30;
  
  return angle;
  } else{
    
  return 0;
  }

}

// convert the angle between the period of PWM signal to angle for tilt panel

double dutycycle_conversion_tilt(volatile float angle){
  float dutycycle = 0;
  // the 0 deg corresponding to 0.122 and 180 deg corresponding to 0.027.
  dutycycle = ((0.122-0.027)*(angle/180)) + 0.027;
  return dutycycle;
  
}
// convert the angle between the period of PWM signal to angle for pan panel

double dutycycle_conversion_pan(volatile float angle){
  // the 0 deg corresponding to 0.114 and 180 deg corresponding to 0.021.
  float dutycycle = 0;
  dutycycle = ((0.114-0.021)*(angle/180)) + 0.021;
  return dutycycle;
}
  

void PWM_config (void){
  PWMCLK = 0x00;      //select clock A as the clock source
  PWMPRCLK = 0x44;    //select the prescaler to be 16.    
  PWMPOL = 0xA0;      //set the input value high.
  PWMCTL = 0xC0;    // enable 16bit mode to read and wait
  PWMCAE = 0;           //left-aligned mode
  PWMPER67 = 30000;   //24mhz to 50 hz is 16*30000 for twisting;
  PWMPER45 = 30000;   // for heading
  PWMDTY67 = 1350;
  PWMDTY45 = 1950;    //set the duty cycle to 6.5% to maintain 60 degree.
  PWME=0xA0;       //enable pp7 and pp5
  
}

void motor(volatile double dutycycle_h,volatile double dutycycle_t){
  PWMCLK = 0x00;      //select clock A as the clock source
  PWMPRCLK = 0x44;    //select the prescaler to be 16.    
  PWMPOL = 0xA0;      //set the input value high.
  PWMCTL = 0xC0;    // enable 16bit mode to read and wait
  PWMCAE = 0;           //left-aligned mode
  PWMPER67 = 30000;   //24mhz to 50 hz is 16*30000 for twisting;
  PWMPER45 = 30000;   // for heading
  PWMDTY67 = 30000*dutycycle_t;
  PWMDTY45 = 30000*dutycycle_h;    //set the duty cycle to 6.5% to maintain 60 degree.
  PWME=0xA0;       //enable pp7 and pp5

}