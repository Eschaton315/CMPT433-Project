#ifndef _DISTANCE_SENSOR_H_
#define _DISTANCE_SENSOR_H_

#include <stdbool.h>

#define DISTANCE_SENSOR_ADDR 0x29  // Component address

/*
Header file for running the Distance Sensor
This module uses the tof library to setup and read values from the tof sensor
Repo found here: https://github.com/bitbank2/VL53L0X
The library must be imported in order for this module to work.
*/

// DS_init() must be called before calling any other function in this library
// init() initializes Distance Sensor by configuring I2C pins and calling tofInit()
// init() also spawns a thread to enable distance reading
void DS_init();

// DS_cleanup() must be called in the cleanup phase of the driver program
// cleanup() sets the distance sensor driver flag to false and attmepts to join the sensor thread
void DS_cleanup();

// Returns distance value
float distance_getData();

#endif