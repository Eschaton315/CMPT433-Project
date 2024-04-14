#include "hal/motor.h"

#include <pthread.h>

#include "hal/shared.h"

pthread_t MotorThreadID;

void *motor_thread(void *);

// Configures the GPIO pin for sourcing power
void MOTOR_init(void) {
  runCommand("config-pin p9_15 gpio");
  runCommand("echo out > /sys/class/gpio/gpio48/direction");
  pthread_create(&MotorThreadID, NULL, &motor_thread, NULL);
}

// Vibrates for 1 sec when the motor flag is set
void *motor_thread(void *arg) {
  (void)arg;
  while (!Get_Terminate()) {
    if (Get_motor_flag()) {
      MOTOR_vibrate(1);
    }
  }
  return NULL;
}

// Low-level control function to vibrate the motor
void MOTOR_vibrate(int time_in_sec) {
  runCommand("echo 1 > /sys/class/gpio/gpio48/value");
  sleepForMs(time_in_sec * 1000);
  MOTOR_stop();
}

// Low-level control function to stop vibrating the motor
void MOTOR_stop(void) { runCommand("echo 0 > /sys/class/gpio/gpio48/value"); }

// Clean-up function
void MOTOR_cleanup(void) { pthread_join(MotorThreadID, NULL); }