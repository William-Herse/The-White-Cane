/*
* Buzzer functions
* Author: William Herse Locke Howard Jiayi Han
* Date: June 01, 2021

  The output compare of TC6 is initialised.
  The timers used on channel 6 are initialised to use the interrupts
  The PWM signal is varied in order to produce a frequency on the dragon boards speaker.
*/

#include <hidef.h>      /* common defines and macros */

#include "buzzer.h"
#include "stdio.h"

// include the register/pin definitions
#include "derivative.h"      /* derivative-specific definitions */

// initalise the output compare of TC6
void Init_Buz() {
  
    // set up the timers for channel 6 to use the interrupt
  TSCR1 = 0x90;
  TSCR2 = 0b00000000;
  TTOV = 0b11000000;
  TIOS = 0b11000000;
  TIE = 0b11000000;
  // set up speaker
  DDRT = 0b00100000;
  TC6 = 15000;
  
}

int Buz_Enable(int buzFlag) {
 
     Init_Buz();
 
     if (buzFlag == 1) {
        
        DDRT = 0b00100000;
        return 1;
      
     } else if (buzFlag == 0) {
        
        DDRT = 0x00;
        return 0;
     }
  
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void Buz_ISR(void) { 
  
  // set the next timer compare value to output required frequency for note
  TC6 = TCNT + 10000;
  TFLG1=TFLG1 | TFLG1_C6F_MASK;
  
  // flip all bits in port PTT
  
  PTT = ~PTT;
  
  // divide everything by 00100000
  
  PTT = PTT & 0x20;
  
}