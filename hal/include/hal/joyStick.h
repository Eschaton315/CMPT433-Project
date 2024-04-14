#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include "hal/shared.h"


//configure pin to GPIO
void configPinGPIO();

//Initiates the thread for joystick after configuring GPIO
void joystick_init();

//Returns boolean of whether a joystick of designated path is pressed
bool joystickPressed(char *path);

//Cleanup function of joystick thread
void joystickListener_cleanup();

//returns which direction the joystick is pressed
int joystick_getJoystickValue();

#endif