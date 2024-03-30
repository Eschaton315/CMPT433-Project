//Derived from: https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
//and https://github.com/jrowberg/i2cdevlib/tree/master/PIC18 
//relevant datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

//#include "hal/getbno080.h"
#include "hal/shared.h"
#include "hal/I2Cdev.h"
#include "hal/MPU6050.h"

/* for the BNO085
#define I2C_PATH "/dev/i2c-2"
#define addr 0x4A
#define READ_SIZE 20
*/
#define I2C_ADDR 0x68

/* int16_t ax,ay,az,gx,gy,gz;
 float accX,accY,accZ,gyroX,gyroY,gyroZ;
 float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
 accAngleX=accAngleY=gyroAngleX=gyroAngleY=gyroAngleZ=0;
 float roll, pitch, yaw;
 roll = pitch = yaw = 0;
*/


long long prevTime ;
long long currentTime = 0;
float accAngleX = 0;
float accAngleY = 0;
float accAngleZ = 0;
float gyroAngleX = 0;
float gyroAngleY = 0;
float gyroAngleZ = 0;

float roll = 0;
float pitch = 0;
float yaw = 0;


bool gyro_init(void) {
    runCommand("config-pin p9.19 i2c");
    runCommand("config-pin p9.20 i2c");
    currentTime = getTimeInMs();
    I2Cdev(2);
    MPU6050(I2C_ADDR);
    MPU6050_initialize();
    return MPU6050_testConnection();
}

//Derived from: https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
//and https://github.com/jrowberg/i2cdevlib/tree/master/PIC18 
//relevant datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf 
void gyro_readData(){
    //initialize values
    int16_t ax,ay,az,gx,gy,gz;
    float accX,accY,accZ,gyroX,gyroY,gyroZ;

    //get Time offset     
    prevTime = currentTime;
    currentTime = getTimeInMs();
    float elapsedTime = (currentTime - prevTime);
    elapsedTime = elapsedTime / 1000;

    MPU6050_getMotion6(&ax,&ay,&az,&gx,&gy,&gz); //there is probably a problem with this line.

         //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
    accX = (float)ax/16384.0;
    accY = (float)ay/16384.0;
    accZ = (float)az/16384.0;
    // the wrong code can also be here?
    accAngleX = (atan(accY / sqrt(pow(accX, 2) + pow(accZ, 2))) * 180 / M_PI) - 0.58;
    accAngleY = (atan(-1 * accX / sqrt(pow(accY, 2) + pow(accZ, 2))) * 180 / M_PI) + 1.58;

        //commented out error correction
        // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
    gyroX = ((float)gx/131); //+0.56;
    gyroY = ((float)gy/131); //-2;
    gyroZ = ((float)gz/131); //+0.79;

    gyroAngleX = gyroAngleX + gyroX * elapsedTime; 
    gyroAngleY = gyroAngleY + gyroY * elapsedTime;

    yaw = yaw + gyroZ * elapsedTime; 
    roll = (0.96 * gyroAngleX);// + (0.04 * accAngleX);
    pitch = (0.96 * gyroAngleY);// + (0.04 * accAngleY);

    printf("yaw: %.3f roll: %.3f pitch: %.3f\n", yaw,roll,pitch);


/* For BNO085
    int file;
    bool mode = false;
   char buf[2];  
   char data[READ_SIZE];  

    if ((file = open(I2C_PATH, O_RDWR)) < 0) {
        perror("Failed to open the bus.");
        exit(1);
    }

    // Set I2C device address
    
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("Failed to acquire bus access and/or talk to slave.");
        close(file);
        exit(1);
    }

    if(mode){
        //ONLY PRINTS OUT 00's
        //print_gyr_conf();
        size_t bytesRead = read(file, data,sizeof(data));

                if (bytesRead > 0) {
               // printf("Read %zu bytes in hexadecimal format:\n", bytesRead);
                for (size_t i = 0; i < bytesRead; i++) {
                  //  printf("%02X ", data[i]); // Print each byte in hexadecimal format
                }
                printf("\n");
            } else {
                printf("Error reading file.\n");
            }
    }else{

        buf[0] = 0xF9;  // Command byte according to the documentation, should let me access the gyro data.
        buf[1] = 0x00;
        // I dont know what i am supposed to write for the second byte since the documentation only says "reserved"
        if (write(file, buf, 2) != 2) {
            perror("Failed to write to I2C bus.");
            close(file);
            exit(1);
        }



        if (read(file, data, READ_SIZE) != READ_SIZE) {
            perror("Failed to read from I2C bus.");
            close(file);
            exit(1);
        }

        // Close I2C bus
        close(file);
        printf("Raw Gyroscope Data? (Hexadecimal):\n");
        for (int i = 0; i < READ_SIZE; i++) {
            printf("%02X ", data[i]);  // Print each byte in hexadecimal format
        }
        printf("\n");
    }
    */
}
