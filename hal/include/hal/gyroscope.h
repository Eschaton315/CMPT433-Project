#ifndef _GYROSCOPE_H_
#define _GYROSCOPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <pthread.h>


#include "hal/shared.h"
#include "hal/I2Cdev.h"
#include "hal/MPU6050.h"

//Derived from: https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
//and https://github.com/jrowberg/i2cdevlib/tree/master/PIC18 
//relevant datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf 

void gyro_init(void);

void gyro_readData();

float* gyro_getData();

void gyro_cleanup();

#endif