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

void savitzky_golay_smooth(float data[]);

void Collect_Initial_Samples();

void Collect_Sample();

void Organize_init();

void Smooth_Data();

float* get_smoothed_gyroData();

float* get_smoothed_distanceData();

#endif