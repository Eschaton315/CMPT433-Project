#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <math.h>

#include "foo.h"
#include "hal/pwm.h"
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
  bool fall  = false;
  long long fallTimer = 0;
  long long currentTime = 0;

  if(DISTANCE_SENSOR){
    DS_init();
  }
  if(GYROSCOPE){
    gyro_init();
  }
  
  joystick_init();
  configBuzzer();
  

  // Let Gyro Run until right is clicked on the joystick
  while(joystick_getJoystickValue()!=4){
    gyroData = gyro_getData();
    yaw = gyroData[0];
    roll = gyroData[1];
    pitch = gyroData[2];
    
    printf("Yaw: %0.2f Roll: %0.2f  Pitch: %0.2f \n",yaw,roll,pitch);
    if(yaw>70||yaw<-70||pitch>70||pitch<-70){

      if(!fall){
        fall = true;
        fallTimer = getTimeInMs();
      }else{
        currentTime = getTimeInMs();
        if(currentTime-fallTimer>1500){
          //if the stick is parallel to the ground for over 1.5 seconds the stick will recognize a fall
          while(joystick_getJoystickValue()!=3){
            printf("FALLEN\n");
          }
          fall = false;
        }
      }
    }else{
      fall = false;
    }
    /*
    if(joystick_getJoystickValue() == 5){
      //recalibrate joystick
      gyro_cleanup();
      printf("RECALIBRATE GYRO");
      gyro_init();
    }
    */
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