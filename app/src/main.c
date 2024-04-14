#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <math.h>

#include "foo.h"
#include "organize.h"
#include "pwmDriver.h"
#include "hal/motor.h"
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

  //Since the beaglebone is upside down on the cane, the comments will refer to the direction referred in the code.
  //The following print statement will show the opposite to reflect the orientation of the beaglebone.

  printf("UP: Stop alarm\n");
  printf("RIGHT: Recalibrate Gyro\n");
  printf("LEFT Stop Program\n");

  //initialize hardware

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
    if (distance < 50.000){
      Change_motor_flag(true);	  
	  }	else{
      Change_motor_flag(false);
    }

    //prints gyro value
    printf("Yaw: %0.2f Roll: %0.2f  Pitch: %0.2f distance: %0.2f\n",yaw,roll,pitch,distance);

    //If yaw, roll, or pitch is over a set value, wait if it stays in that range for 1.5 seconds to detect a fall.
    if(yaw>70||yaw<-70||roll>70||roll<-70||pitch>70||pitch<-70){
	  
      //Detect initial fall threshold
      //If the stick passes the threshold for over 1.5 seconds the stick will recognize a fall
      if(!fall){

        fall = true;
        fallTimer = getTimeInMs();

      }else{

        currentTime = getTimeInMs();

        if(currentTime-fallTimer>1500){
          
          //A fall has been detected and will start a buzzer to initiate alarm
          BuzzerMissThreadCreate();
          buzzerTimer = getTimeInMs();
          //Print FALLEN until down joystick is pressed to cancel the alarm
          
          while(joystick_getJoystickValue()!=2){

            currentTime = getTimeInMs();

            if(currentTime-buzzerTimer>1000){

              BuzzerMissThreadCreate();
              printf("FALLEN\n");
              buzzerTimer = currentTime;

            }
          }
          //Alarm turned off
          fall = false;
        }
      }
    }else{
      //Threshold not maintained so alarm will not start
      fall = false;
    }
    
    //Recalibrate gyro value to 0 if joystick is pressed left
    //Clean up and reinitiate the gyro thread
    //Set halt to prevent other functions from reading gyro data
    if(joystick_getJoystickValue() == 3){
      printf("RECALIBRATE GYRO KEEP CANE STILL\n");
      Change_halt(true);
      sleepForMs(100);
      gyro_cleanup();
      
      gyro_init();
      sleepForMs(100);
      Change_halt(false);
    }
    sleepForMs(100);
  }

  //Start clean up
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