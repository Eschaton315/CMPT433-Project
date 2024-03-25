#include <stdio.h>

#include "foo.h"
#include "hal/distanceSensor.h"
#include "hal/shared.h"
int main() {
  DS_init();
  // Let DS run for 10 seconds
  sleepForMs(10000);
  DS_cleanup();
  printf("Main program Finished, exiting. . .\n");
  return 0;
}