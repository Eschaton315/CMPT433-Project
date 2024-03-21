#include <stdio.h>

#include "foo.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
int main() {
  DS_init();
  while (1) {
    printf("Hello World\n");
    printf("Distance = %d\n", DS_getReading());
    sleepForMs(1000);
  }
  return 0;
}