#ifndef BUZZER_HEADER
#define BUZZER_HEADER

// flag for whether the buzzer should be on or off
extern int buzFlag;

// function which configures registers for the timer and the buzzer
void Init_Buz(void);

// function which enables and disables the buzzer
int Buz_Enable(int buzFlag);

// interrupt which is used to toggle the PT5 and make the buzzer produce sound
__interrupt void Buz_ISR(void);   


#endif