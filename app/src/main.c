#include <stdio.h>
#include <stdbool.h>

#include "foo.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
int main() {
  bool sensor = false;
  
  if(sensor){
    DS_init();
    while (1) {
    printf("Hello World\n");
    printf("Gyro Test");
    printf("Distance = %d\n", DS_getReading());
    sleepForMs(1000);
  }
  }

  return 0;
}