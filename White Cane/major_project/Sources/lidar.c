/*
* lidar measure functions
* Author: William Herse Locke Howard Jiayi Han
* Date: June 01, 2021

  The lidar works by capturing the rising and falling edge of the PWM signal that corresponds the light beam being sent and then received in the sensor.
  The signal is captured from pin 1, where the code waits for the rising edge to be received.
  When the rising edge is recieved it is saved the rise variable
  The timer is then continously running, flags are cleared
  The code then waits for the falling edge. When received it is saved to the drop variable.
  The difference between the two is calculated, and the number of overflows is recorded
  If the timer has overflowed, and the rise is higher than the drop the difference is changed accordingly.
  The difference is then passed back to the lidarRun function
  The lidar distance is sampled 10 times
  The array of values is sorted based on size, the median of this is then found, and the overflow value is averaged
  The final distance is calculated with the median and average, being returned to the main function 
*/



#include <hidef.h>
#include "lidar.h"
#include "derivative.h"
#include <math.h>
#include "simple_serial.h"

unsigned int overflow = 0;
unsigned int overflowStore = 0;
unsigned long int pulseValue[10];
unsigned int diff;
unsigned long int edge1, pulsewidth;

//overflow must be a global variable

unsigned long int lidar(void) {

  unsigned int rise=0,drop = 0;
                                   
  overflow = 0;

 
  TFLG1 &= 0xFD;                     // clear C1F flag

  TCTL4 = 0x04;                     // allow capture of rising edge on pin 1

  while(!(TFLG1 & 0x02));           // wait for rising edge

  TSCR2 = 0b10000011;

  rise = TC1;                       // save the first edge (rising edge)
                                    
  asm("cli");                       // clear the interrupt flag
  
  TFLG1 &= 0xFD;                    // clear C1F flag

  TCTL4 = 0x08;                     // allow capture of falling edge on pin 1

  while (!(TFLG1 & 0x02));          // wait for falling edge

  drop = TC1;                       // save the second edge (falling edge)

  TFLG1 = 0x02; 
  
  TSCR2 = 0b10000011;

  diff = drop - rise;               // calculate the difference

  overflowStore = overflow;         // store the overflow variable

  overflowStore = ceil(overflowStore/100);

  // if the difference is negative, decrement the overflow count
  // up to one overflow
  if (rise > drop && overflow > 0) {
    diff = (((32768)-rise)+drop);
    overflowStore -= 1;
  }

  

  return diff;     // returned dist
  

}

void Array_sort(long int *array , int n) {
 
    // declare some local variables
    int i=0 , j=0 , temp=0;

    for(i=0 ; i<n ; i++)
    {
        for(j=0 ; j<n-1 ; j++)
        {
            if(array[j]>array[j+1])
            {
                temp        = array[j];
                array[j]    = array[j+1];
                array[j+1]  = temp;
            }
        }
    }


}

// function to calculate the median of the array
long int Find_median(long int array[] , int n) {
    
    int median=0;
    
    // if number of elements are even
    if(n%2 == 0)
        median = (array[(n-1)/2] + array[n/2])/2;
    // if number of elements are odd
    else
        median = array[n/2];
    
    return median;
}


unsigned long int lidarRun(void){

  int i = 0;                                                 //local variables used in sampling the lidar data
  unsigned long int medianP, medianO; 
  unsigned long int pulseValue[10];
  unsigned long int overflowValue;
  unsigned long int distance = 0;

  for(i = 0; i < 10; i++){                                   //the distance is sampled 10 times, with the overflow values stored in one holding variable
    pulseValue[i] = lidar();
    overflowValue = overflowValue + overflowStore;
  }
  Array_sort(pulseValue, 10);                                //the array of samples is sorted
  medianP = Find_median(pulseValue, 10);                     //the median is found in the sorted array
  
  overflowValue = overflowValue/10;                          //the overflow value is averaged
  
  //Array_sort(overflowValue, 10);
  //medianO = Find_median(overflowValue, 10);
  
  //calculate the pulsewidth
  //distance = overflowValue * 372; 
  distance = distance + medianP/10;                          //the distance is calculated with the median and averaged overflow
  
  return distance;
  

  //use serial function to display the distance, change size if required but should be fine otherwise check memory for location of the string.
  //this function will continue to run until program is terminated by user, constantly changing the distance string to show the current value.
}


void Init_TOF(void) {
  
  TSCR1 = 0x90;
  TSCR2 = 0b10000011;
  TTOV = 0b11000000;
  TIOS = 0b11000000;
  
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void tovisr(void) {

  // set the next timer compare value to output required frequency for note

  TFLG2 = 0x80;
  overflow++;   // increment the overflow

}
