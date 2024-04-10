#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <math.h>

#include "foo.h"
#include "pwmDriver.h"
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
#define I2C_ADDR 0x68


int main() {

  float *gyroData;
  float yaw,pitch,roll;
  //int distance;
  bool fall  = false;
  long long fallTimer = 0;
  long long currentTime = 0;
  long long buzzerTimer = 0;

  //initialize hardware

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
    //distance = DS_getReading();

    //prints gyro value per 0.1 sec.
    printf("Yaw: %0.2f Roll: %0.2f  Pitch: %0.2f \n",yaw,roll,pitch);
    if(yaw>70||yaw<-70||pitch>70||pitch<-70){
      //if yaw or pitch is over a set value, wait if it stays in that range to detect a fall.
      if(!fall){
        fall = true;
        fallTimer = getTimeInMs();
      }else{
        currentTime = getTimeInMs();
        if(currentTime-fallTimer>1500){
          BuzzerMissThreadCreate();
          buzzerTimer = getTimeInMs();
          //if the stick is parallel to the ground for over 1.5 seconds the stick will recognize a fall
          //The code willl print FALLEN until left joystick is pressed
          while(joystick_getJoystickValue()!=3){
            currentTime = getTimeInMs();
            if(currentTime-buzzerTimer>1000){
              BuzzerMissThreadCreate();
              printf("FALLEN\n");
              buzzerTimer = currentTime;
            }

          }
          fall = false;
        }
      }
    }else{
      fall = false;
    }
    
    //recalibrate gyro value to 0 if joystick is pressed in
    if(joystick_getJoystickValue() == 5){
      gyro_cleanup();
      printf("RECALIBRATE GYRO\n");
      gyro_init();
    }
    
    sleepForMs(100);
  }

  BuzzerMissThreadJoin();
  if(DISTANCE_SENSOR){
    DS_cleanup();
  }
  if(GYROSCOPE){
    gyro_cleanup();
  }
  printf("Main program Finished, exiting. . .\n");
  return 0;
}