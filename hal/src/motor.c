#include "hal/motor.h"

#include "hal/shared.h"

void MOTOR_init(void) {
  runCommand("config-pin p9_48 gpio");
  runCommand("echo out > /sys/class/gpio/gpio48/direction");
}

void MOTOR_vibrate(int time_in_sec) {
  runCommand("echo 1 > /sys/class/gpio/gpio48/value");
  sleepForMs(time_in_sec * 1000);
  MOTOR_stop();
}

void MOTOR_stop(void) { runCommand("echo 0 > /sys/class/gpio/gpio48/value"); }