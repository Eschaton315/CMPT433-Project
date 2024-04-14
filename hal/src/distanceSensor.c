#include "hal/distanceSensor.h"

#include <pthread.h>
#include <stdio.h>
#include <tof.h>

#include "hal/shared.h"

volatile bool DS_DRIVER_FLAG = true;
static pthread_t sensorThreadID;

float distance;

void *sensor_Thread(void *);

// Current system uses P9.17/18 for I2C, must change these if using different pins
// Refer to BeagleBone manual for correct pin usage
void DS_init(void) {
  runCommand("config-pin p9.17 i2c");
  runCommand("config-pin p9.18 i2c");
  tofInit(1, DISTANCE_SENSOR_ADDR, 0);  // Call tof library init function
  printf("DS_init called\n");
  pthread_create(&sensorThreadID, NULL, sensor_Thread, NULL);
}

void DS_cleanup(void) {
  printf("DS_cleanup called\n");
  DS_DRIVER_FLAG = false;
  pthread_join(sensorThreadID, NULL);
  printf("DS_cleanup finished\n");
}

// Wrapper function to call tofReadDistance() from tof library
static int DS_getReading() { return tofReadDistance(); }

/*
This thread periodically updates the distance variable every 50 milliseconds
Thread stops when the Distance Sensor Driver flag is set to false
*/
void *sensor_Thread(void *arg) {
  (void)arg;
  while (DS_DRIVER_FLAG) {
    distance = (float)DS_getReading() / 10;
    sleepForMs(50);
  }
  return NULL;
}

// Getter function for the distance reading
float distance_getData() { return distance; }