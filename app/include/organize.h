#ifndef _ORGANIZE_H_
#define _ORGANIZE_H_

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "hal/gyroscope.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"

//Use moving average to smooth the array
void moving_average_smooth(float data[]);

//Collect a sample of data from gyroscope of Yaw, Roll, and Pitch
//Along with a sample of data from the distance sensor and saves it
void Collect_Sample();

//The initializing function for running the data organizer
void Organize_init();

//call the smoothing algorithm on the data and store them
void Smooth_Data();

//Get the smoothed gyroscope data for use
float* get_smoothed_gyroData();

//Get the smoothed distance data for use
float get_smoothed_distanceData();

//Clean up for organize thread
//Must be called at the end
void organize_cleanup();

#endif