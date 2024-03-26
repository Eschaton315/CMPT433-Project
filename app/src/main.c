#include <stdio.h>
#include <stdbool.h>

#include "foo.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
#include "hal/gyroscope.h"
#include "hal/sh2.h"
int main() {
  bool sensor = false;
  printf("Gyro Test\n");
  sh2_Hal_t* sh2Gyro = sh2_hal_init();
  int status = sh2_open(sh2Gyro, eventHandler, NULL);
  sh2_setSensorCallback(sensorHandler,NULL);
  


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