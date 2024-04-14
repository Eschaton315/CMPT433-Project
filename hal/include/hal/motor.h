#ifndef _MOTOR_H_
#define _MOTOR_H_

// Configures the GPIO pin and set the direction to out
void MOTOR_init(void);

// Low level function to make the motor vibrate in seconds
void MOTOR_vibrate(int time_in_sec);

// Low level function to make the motor stop vibrating
void MOTOR_stop(void);

#endif