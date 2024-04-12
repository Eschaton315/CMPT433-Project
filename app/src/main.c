#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <math.h>

#include "foo.h"
#include "organize.h"
#include "hal/motor.h"

#include "pwmDriver.h"
#include "hal/pwm.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
#include "hal/gyroscope.h"
#include "hal/I2Cdev.h"
#include "hal/MPU6050.h"
#include "hal/joyStick.h"

//Change value depending on whether the hardware is installed or not.
#define DISTANCE_SENSOR true
#define GYROSCOPE true
#define I2C_ADDR 0x68


int main() {

  float *gyroData;
  float yaw,pitch,roll;
  float distance;
  bool fall  = false;
  long long fallTimer = 0;
  long long currentTime = 0;
  long long buzzerTimer = 0;

  //initialize hardware

  printf("DOWN: Stop alarm\n");
  printf("LEFT: Recalibrate Gyro\n");
  printf("RIGHT: Stop Program\n");

  if(DISTANCE_SENSOR){
    DS_init();
  }
  if(GYROSCOPE){
    gyro_init();
  }
  
  
  MOTOR_init();
  Organize_init();
  joystick_init();
  configBuzzer();

  // Let Gyro Run until right is clicked on the joystick
  while(joystick_getJoystickValue()!=4){
    gyroData = get_smoothed_gyroData();
    yaw = gyroData[0];
    roll = gyroData[1];
    pitch = gyroData[2];
    distance = get_smoothed_distanceData();

    //prints gyro value per 0.1 sec.
    printf("Yaw: %0.2f Roll: %0.2f  Pitch: %0.2f distance: %0.2f\n",yaw,roll,pitch,distance);
    if(yaw>70||yaw<-70||roll>70||roll<-70||pitch>70||pitch<-70){


	  if (distance < 30.000){
			MOTOR_vibrate(1);		  
	  }	  
	  
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
          //The code willl print FALLEN until down joystick is pressed
          while(joystick_getJoystickValue()!=2){
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
    
    //recalibrate gyro value to 0 if joystick is pressed left
    if(joystick_getJoystickValue() == 3){
      printf("RECALIBRATE GYRO KEEP CANE STILL\n");
      change_halt(true);
      sleepForMs(100);
      gyro_cleanup();
      
      
      gyro_init();
      sleepForMs(100);
      change_halt(false);
    }
    
    sleepForMs(100);
  }

  BuzzerMissThreadJoin();
  organize_cleanup();
  
  if(DISTANCE_SENSOR){
    DS_cleanup();
  }
  if(GYROSCOPE){
    gyro_cleanup();
  }
  printf("Main program Finished, exiting. . .\n");
  return 0;
}