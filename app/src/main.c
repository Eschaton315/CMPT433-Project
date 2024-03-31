#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <math.h>

#include "foo.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
#include "hal/gyroscope.h"
#include "hal/I2Cdev.h"
#include "hal/MPU6050.h"

//Change value depending on whether the hardware is installed or not.
#define DISTANCE_SENSOR false
#define GYROSCOPE true
//#include "hal/getbno080.h"
#define I2C_ADDR 0x68


int main() {
  if(DISTANCE_SENSOR){
    DS_init();
  }
  if(GYROSCOPE){
    gyro_init();
  }
  
  // Let DS run for 10 seconds
  sleepForMs(10000);
  if(DISTANCE_SENSOR){
    DS_cleanup();
  }
  if(GYROSCOPE){
    gyro_cleanup();
  }
  printf("Main program Finished, exiting. . .\n");
  return 0;
}