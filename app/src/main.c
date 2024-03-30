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
//#include "hal/getbno080.h"
#define I2C_ADDR 0x68


int main() {
  bool sensor = false;
  
  printf("Gyro Test\n");
  
  if(!gyro_init()){
    perror("Failed to access the gyroscope.");
    exit(1);
  }

  while(1){
    sleepForMs(1000);
    gyro_readData();
  }

  if(sensor){
    //DS_init();
    while (1) {
    printf("Hello World\n");
    
    //printf("Distance = %d\n", DS_getReading());
    sleepForMs(1000);
  }
  }

  return 0;
}