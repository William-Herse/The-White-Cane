double angle_measured(volatile float duty_cycle);
double dutycycle_conversion_tilt(volatile float angle);
double dutycycle_conversion_pan(volatile float angle);
void PWM_config(void);
void motor(volatile double dutycycle_h,volatile double dutycycle_t);