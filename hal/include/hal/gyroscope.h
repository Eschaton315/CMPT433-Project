#ifndef _GYROSCOPE_H_
#define _GYROSCOPE_H_

//Derived from: https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
//and https://github.com/jrowberg/i2cdevlib/tree/master/PIC18 
//relevant datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf 

bool gyro_init(void);

void gyro_readData();

#endif