#ifndef _SHARED_H_
#define _SHARED_H_

#include <assert.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

/*
Central shared module to be used in other hal modules + main
This module is designed to define any functions that are used by multiple modules within the system
*/

// Runs provided termminal command
void runCommand(char *command);

// Returns system real time clock in nanoseconds
long long getTimeInMs(void);

// Sleeps the calling thread for specified amount of milliseconds
void sleepForMs(long long delayInMs);

//Sets specified I2C device address to slave address 
// Code taken from Dr. Brian Fraser's I2C guide
// Guide found here: https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/guides
int initI2cBus(char *bus, int address);

// Writes the value into the designated i2c device register address 
// Must provide a valid i2cFileDescriptor and valid register address
// Code taken from Dr. Brian Fraser's I2C guide
void writeI2cReg(int i2cFileDesc, unsigned char redAddr, unsigned char value);

// Returns contents of the designated I2C device register address
// Code taken from Dr. Brian Fraser's I2C guide
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

// Writes contents into the designated file
// Mimics the functionality of "echo" command
void EchoToFile(char* filePath, char* contents);

//Changes the terminate flag to status
void Change_Terminate(bool status);

// Returns the value of terminate flag
bool Get_Terminate();

// Changes the halt flag to status
void change_halt(bool status);

// Returns the value of halt flag
bool get_halt();

// Changes the value of motor flag to status
// Used by the motor module to initiate driving the vibration motor
void change_motor_flag(bool status);

// Returns the value of motor flag
bool get_motor_flag();

#endif