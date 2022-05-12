


# MTRX2700 Major

## Major project - The White Cane

## Team
> Will Herse, Harry Han, Locke Howard

> 480394967 490019296 490422164


## Idea
Our idea is an assistive scanning device for visually impaired individuals that warns individuals of objects or hazards in their path.

>- Device will auto stabilise always looking at 50 degree angle to ground
>- Will scan a 110 degree arc in front of user
>- When an object or hazard is detected the buzzer will sound to warn the user


## Modules

The project employs the use of 4 main modules:

 >- Accelerometer: the accelerometer is used to keep the device stable and always looking directly in front of the user at a 50 degree angle.
 >	  - Contributors: Harry, Locke
 >- Lidar: The lidar scanner is used to return the distance from the objects to the individual’s midline where the device is situated, used to warn of objects or hazards.
 >	  - Contributors: Locke, Will
 >- Buzzer: the buzzer is triggered to warn the user of the approaching object or hazards.
 >	  - Contributors: Will
 >- Servo (Gyro): The servo is what moves the scanner around, used to stabilise the scanner so that is remains looking in the same direction and to sweep across the arc in front of the user.
 >	  - Contributor: Harry, Locke

## Stabilisation

### Summary

The accelerometer is utilised to keep the scanner stable and always in front of the user, this is achieved through the data it outputs on the current state of the sensor. The device was calibrated at rest, assuming stable velocity with no change in acceleration with only the effect of gravity to account for, looking down at a 50 degree angle. The acceleration vector that it outputs was taken as our frame of reference and desired vector that we want the device to be under. When the device is running the current vector that the accelerometer is outputting is sent to a function that equates the difference in angle between the that and the desired vector. This angle is then added/subtracted from the current angle depending upon the value for the y vector, in order to correct the device and stabilise it.

The gyroscope is also used to stabilise the scanner. It functions through the measurements taken by the gyroscope instrument in the hardware, and converts them into an angle that can be used to counteract any movement sustained by outside forces. The device doesnt require specific calibration, only so far as to be aware of what values correspond to degree of movement in order to return the correct value.


### Usage
The function `angleConversion` is the basis for how we utilise the accelerometer. it uses the raw data received from the device by using `scaled_accel.x/y/z` and calculates the angle between the current acceleration vector and the desired vector.

The calculation that is used is as follows:

![accelerometer eq](https://github.com/lhow8475/MTRX2700-Major/blob/main/readme%20pictures/accelerometer%20equation.PNG)

Changing the variables `desiredX` and `desiredZ` will change the value of the desired vector and have the device stabilise to a different position.

In order to choose this desired vector the device must be calibrated at rest, measuring the output when it is in its desired position. Changing the desired vector to this value will have it stabilise to that position.

The gyroscope functions through the function `anglemeasure` that provides the angle required to counteract the movement. This is is then subtracted from the current angle and the previous position in order to account for the results the gyro will report when the system is moved purposefully which can affect the accuracy of the stabilisation.

### Valid input

The variables into the function are doubles, whilst the returned value is of type int.

`anglemeasure` takes in an int that is received from the `read_gyro.y` variable.

### Functions and modularity

- angleConversion(function)
	- takes desired vector for the x-z axis and the current vector on the x-z axis and computes the angle between these two functions
- angleMeasure(function)
	- Converts the raw gyro readings to a angle value

The functions both return the difference in angle, this is then used accordingly.

### Testing

The device was calibrated to various angles and then moved around to ensure it was acting in the desired manner, including being fully rotated to ensure it could handle all 3D positions

## Lidar

### Summary

The lidar is utilised to output the current distance from the midline of the user where the device is to the floor at a 50 degree angle. The device is first calibrated by having it sit and scan the arc in front of the user to a flat floor. The distances received are used as the parameters deemed safe. When the device is in use any distance that is returned that is outside of this range triggers the buzzer. It works through reading the PWM signal taken from the lidar sensor. When the rising edge is given indicating that a light signal has been sent from the device the current timer value is captured. The timer is then allowed to tick up with appropriate overflow interrupts in use. When the signal is received back indicating that it had reflected from a physical object then returned to the sensor the falling edge is given and the timer value captured. The distance is then calculated from the timer and overflow values, a sample size of 5 is taken and returned.

### Usage
The functions `lidar` and `lidarRun` are the primary points for utilising the lidar, with `lidarRun` the function being called to receive back the distance. When called the a sample of 10 will be taken and put into an array, that array is then sorted to find the median and the average of the overflow values is taken the following calculation is then used to get a value for the distance:

![lidar eq](https://github.com/lhow8475/MTRX2700-Major/blob/main/readme%20pictures/lidar%20equation.PNG)

This distance is then sent back.

In order to be used effectively the device must be calibrated for each individual user as a variance in height will require specific parameters for each individual. To do so the device must be at rest and the user on a flat surface, the device must be allowed to scan the arc in front and find the range of safe values. These will then become the range of safe values, where any value received is outside these parameters the buzzer will be triggered.

### Valid input

The function `lidarRun` takes no various but returns a value of type int.

### Functions and modularity

- Variables
	- overflow, pulsevalues array, time taken
- lidarRun (function)
	- Main function for running the lidar
	- access the other functions to find the distance
	- takes raw values from lidar, finds median of the samples then returns the distance
- lidar (function)
	- measures the time between the rising and falling edge of the PWM signal from the sensor
	- this time can be used quantify the distance between the sensor and the object in front of it
- Array_sort (function)
	- this will sort an array that is passed to it in order of size, must pass in size of the array
- Find_median (function)
	- this will find the median of a sorted array with its included size
- tovisr (interrupt)
	- this handles the timer overflow and increments the overflow counter

### Testing

The device was setup and a sheet held in front of the sensor, this was then slowly moved back and the distance was observed to ensure it was reporting as required. The device was then calibrated for various distances to ensure it would trigger the buzzer when it went above or below the set range.

## Buzzer

### Summary

The buzzer is used to indicate to the user that there is a obstacle detected by the lidar. When the buzzer is enabled, it will produce a sound which can be heard by the operator. The buzzer uses a hardware output compare interrupt to toggle the pin associated with the speaker from high to low at a certain frequency that produces a sound.

### Usage

To use the buzzer, you must initalise it using the function Init_BUZ(). After this, the buzzer can be toggled on and off by the function Buz_enable(Buz_flag) being feed a flag. A flag of 1 enables the buzzer and a flag of 0 disables it.

### Valid input

A valid input into the buzzer enable function is a boolean interger of 0 or 1.

### Functions and modularity

- buzFlag (extern int)
	- a boolean integer used to flag whether the buzzer should sound or not
- Init_Buz (function)
	- used to configure and start the timer
	- used to configure the buzzer registers
- Buz_Enable (function)
	- a function which takes the buzFlag as a input and then enables or disables the buzzer accordingly
- Buz_ISR (function)
	- the interrupt function which is called when the output compare interrupt takes place
	- toggles the speaker pin to produce the output
	- sets the next output compare

### Testing

Toggle buzFlag from 1 to 0 to see if the buzzer is enable or not. If not, measure pins PT5 and DDRT5 with a ocisliscope to ensure that the buzzer is configure properly. If so, set a break point in the interrupt to insure that the output compare is functioning accordingly. 

## Servo

### Summary

The servo is run through the use of a PWM signal. The duty cycle of on over the whole period will decide the angle the servo is going to move to. the motor's running frequeny is 50hz, while the 16 Bit timer inside the board is 24MHZ.  Thus it needs to generate a long enough signal to match the timer frequency.  16*3000 is used to fill the difference. The PWM signal length is timed by 3000. Prescale is 16. The port used are port 5 and port 7. So we need to use a 16bit register for PWM signal to concatenate two ports. which is 45 and 67.  Then control the pwm duty register the desired duty cycle.

### Usage

The servo is primarily run through the function `motor` which takes in the duty cycle for both the pan and tilt that can be received from the two functions `dutycycle_conversion_tilt` and `dutycycle_conversion_pan`. The dutycycle functions will recieve an angle in degrees, which will then be converted to the PWM signal and passed to the motor function.

### Valid input

The dutycycle functions recieve a float variable and will return a double, whilst the motor function will recieve two double variables and return void.

### Functions and modularity

- dutycycle_conversion_tilt (function)
	- converts between the given angle to the dutycycle required by the motor function
- dutycycle_conversion_pan (function)
	- converts between the given angle to the dutycycle required by the motor function
- motor (function)
	- Takes the given dutycycles and pans and tilts the camera accordingly

### Testing

Various angles where passed into the functions and the result was observed to ensure the functions were moving the system to the correct positions.
