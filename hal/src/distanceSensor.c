#include "hal/distanceSensor.h"

#include <tof.h>
#include <stdio.h>
#include <pthread.h>

//temporary insert
#include <time.h> 

#include "hal/shared.h"

volatile bool DS_DRIVER_FLAG = true;
static pthread_t sensorThreadID;

static float distance;

void *sensor_Thread(void *);

void DS_init(void) {
  runCommand("config-pin p9.17 i2c");
  runCommand("config-pin p9.18 i2c");
  tofInit(1, DISTANCE_SENSOR_ADDR, 0);
  printf("DS_init called\n");
  pthread_create(&sensorThreadID, NULL, sensor_Thread, NULL);
}

void DS_cleanup(void){
  printf("DS_cleanup called\n");
  DS_DRIVER_FLAG = false;
  pthread_join(sensorThreadID, NULL);
  printf("DS_cleanup finished\n");
}

//static int DS_getReading() { return tofReadDistance(); }

// Might need a function that gets a reading 10 times a second and returns the average

void *sensor_Thread(void *arg){
  (void) arg;
  srand(time(0)); 
  while(DS_DRIVER_FLAG){
    //Commented temporarily making dummy code
	//distance = (float) DS_getReading() / 10;
	distance = (float)(rand() % (100 - 0 + 1)) + 0;
	
	
    //printf("Distance: %.3f cm\n", distance);
    // get readings every 1 second
    //sleepForMs(1000);
  }
  return NULL;
}

float distance_getData(){
	return distance;
}