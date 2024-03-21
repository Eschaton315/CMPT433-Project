#include "hal/distanceSensor.h"

#include <tof.h>

#include "hal/shared.h"

void DS_init(void) {
  runCommand("config-pin p9.17 i2c");
  runCommand("config-pin p9.18 i2c");
  tofInit(1, 0x29, 0);
}
int DS_getReading() { return tofReadDistance(); }