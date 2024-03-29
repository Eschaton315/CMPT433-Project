#include <stdio.h>
#include <stdbool.h>

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
  gyro_init();
  I2Cdev(2);
  MPU6050(I2C_ADDR);
  MPU6050_initialize();

  bool test = MPU6050_testConnection();

  if(test){
    printf("Gyro range:%d\n",MPU6050_getFullScaleGyroRange());
  }else{
    printf("Naw");
  }
  
 int16_t gx, gy, gz;

  while(1){
        MPU6050_getRotation(&gx, &gy, &gz);
        //printf("Rate:%d\n",MPU6050_getRate());

        // Display tab-separated accel/gyro x/y/z values
        printf("x:%d y:%d z:%d\n", gx, gy, gz);
    sleepForMs(1000);
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