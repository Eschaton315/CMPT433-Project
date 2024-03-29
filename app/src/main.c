#include <stdio.h>
#include <stdbool.h>
#include <sys/ioctl.h>

#include "foo.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
#include "hal/gyroscope.h"
#include "hal/I2Cdev.h"
#include "hal/MPU6050.h"
#include "hal/mpuTest.h"
//#include "hal/getbno080.h"
#define I2C_ADDR 0x68

MPU6050_ACC_DATA_t mpu6050_accData_t;
MPU6050_GYRO_DATA_t mpu6050_gyroData_t;

int fd;

int main() {
  bool sensor = false;
  printf("Gyro Test\n");
  gyro_init();

fd = open(I2C_DEVICE_FILE_PATH,O_RDWR);

    if ( fd < 0) {
        perror("Failed to open I2C device file.\n");
        return -1;
    }

    /*set the I2C slave address  */
    if (ioctl(fd,I2C_SLAVE,MPU6050_SLAVE_ADDRESS) < 0) {
            perror("Failed to set I2C slave address.\n");
            close(fd);
            return -1;
    }

    mpu6050_init();

    while(1) {
    	//mpu6050_readAcc();
    	mpu6050_readGyro();

    	//printf("accX:  %0.2f accY:  %.02f accZ:  %0.2f\n", mpu6050_accData_t.accX, mpu6050_accData_t.accY, mpu6050_accData_t.accZ);
    	printf("gyroX: %0.2f gyroY: %.02f gyroZ: %0.2f\n", mpu6050_gyroData_t.gyroX, mpu6050_gyroData_t.gyroY, mpu6050_gyroData_t.gyroZ);
    	//printf("Temp %0.2f\n\n", mpu6050_readTemp() );
    	sleepForMs(1000);
    }


  /*
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
  
  */

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