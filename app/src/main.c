#include <stdio.h>
#include <stdbool.h>

#include "foo.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
#include "hal/gyroscope.h"

int main() {
  bool sensor = false;
  printf("Gyro Test\n");
  gyro_init();
  
  while(1){

    gyro_readData();
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