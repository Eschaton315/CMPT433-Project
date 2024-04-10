//Derived from: https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
//and https://github.com/jrowberg/i2cdevlib/tree/master/PIC18 
//relevant datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf 



#include "hal/gyroscope.h"

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


static long long prevTime ;
static long long currentTime = 0;
static float accAngleX = 0;
static float accAngleY = 0;
//static float accAngleZ = 0;
static float gyroAngleX = 0;
static float gyroAngleY = 0;
//static float gyroAngleZ = 0;

static float roll = 0;
static float pitch = 0;
static float yaw = 0;

float *gyroData;
float *gyroOffset;

volatile bool GYRO_DRIVER_FLAG = true;
static pthread_t gyroThreadID;
static pthread_mutex_t gyroMutex = PTHREAD_MUTEX_INITIALIZER;

//locks for thread synchronization
static void lock(){
    pthread_mutex_lock(&gyroMutex);
}
static void unlock(){
    pthread_mutex_unlock(&gyroMutex);
}


static void *gyro_Thread();


//Derived from: https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
//and https://github.com/jrowberg/i2cdevlib/tree/master/PIC18 
//relevant datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf 
static void gyro_readData(){
    lock();
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

    gyroData[0] = yaw + gyroOffset[0];
    gyroData[1] = roll+ gyroOffset[1];
    gyroData[2] = pitch+ gyroOffset[2];

    //printf("yaw %0.2f,roll %0.2f,pitch %0.2f\n",yaw,roll,pitch);
    unlock();
}

void gyro_init(void) {
    runCommand("config-pin p9.19 i2c");
    runCommand("config-pin p9.20 i2c");

    gyroAngleX = 0;
    gyroAngleY = 0;
    yaw = 0;
    
    float gyroSum[3] = {0};

    GYRO_DRIVER_FLAG = true;
    currentTime = getTimeInMs();
    gyroData = malloc(3*sizeof(float));
    gyroOffset = malloc(3*sizeof(float));
    memset (gyroOffset, 0, 3*sizeof(float));

    I2Cdev(2);
    MPU6050(I2C_ADDR);
    MPU6050_initialize();

    printf("CALIBRATING\n");
    
    for(int i =0; i<500;i++){
        gyro_readData();
        gyroSum[0] = gyroSum[0] + yaw;
        gyroSum[1] = gyroSum[1] + roll;
        gyroSum[2] = gyroSum[2] + pitch;
    }
    for(int j=0; j<3; j++){
        gyroOffset[j] = gyroSum[j] / 500.0;        
    }
    

    printf("CALIBRATION DONE\n");
    pthread_create(&gyroThreadID, NULL, gyro_Thread,NULL);
    //return MPU6050_testConnection();
}



float* gyro_getData(){
    return gyroData;
}

static void *gyro_Thread(){
    while(GYRO_DRIVER_FLAG){
        gyro_readData();
    }
    return NULL;
}

void gyro_cleanup(){
    printf("gyro_cleanup called\n");
    GYRO_DRIVER_FLAG = false;
    pthread_join(gyroThreadID,NULL);
    free(gyroData);
    free(gyroOffset);
    printf("gyro_cleanup finished\n");
}
