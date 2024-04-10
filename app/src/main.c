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
#include "hal/joyStick.h"

//Change value depending on whether the hardware is installed or not.
#define DISTANCE_SENSOR false
#define GYROSCOPE true
//#include "hal/getbno080.h"
#define I2C_ADDR 0x68


int main() {

  float *gyroData;
  float yaw,pitch,roll;

  if(DISTANCE_SENSOR){
    DS_init();
  }
  if(GYROSCOPE){
    gyro_init();
  }
  
  joystick_init();

  // Let DS run for 10 seconds
  while(joystick_getJoystickValue()!=4){
    gyroData = gyro_getData();
    yaw = gyroData[0];
    roll = gyroData[1];
    pitch = gyroData[2];
    
    printf("Yaw: %0.2f Roll: %0.2f  Pitch: %0.2f \n",yaw,roll,pitch);
    sleepForMs(100);
  }


  if(DISTANCE_SENSOR){
    DS_cleanup();
  }
  if(GYROSCOPE){
    gyro_cleanup();
  }
  printf("Main program Finished, exiting. . .\n");
  return 0;
}