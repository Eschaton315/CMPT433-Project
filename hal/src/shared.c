#include "hal/shared.h"
#include <string.h>

static bool terminate = false;
static bool halt = false;
static bool motor_flag = false;


// Assumes running a linux command on a linux system
void runCommand(char *command) {
  FILE *pipe = popen(command, "r");

  char buffer[1024];
  while (!feof(pipe) && !ferror(pipe)) {
    if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
      break;
    }
  }

  int exitCode = WEXITSTATUS(pclose(pipe));
  if (exitCode != 0) {
    perror("Unable to execute command:");
    printf("%s\n", command);
    printf("With exit code: %d\n", exitCode);
  }
}

long long getTimeInMs()
{
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  long long seconds = spec.tv_sec;
  long long nanoSeconds = spec.tv_nsec;
  long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
  return milliSeconds;
}

void sleepForMs(long long delayInMs) {
  const long long NS_PER_MS = 1000 * 1000;
  const long long NS_PER_SECOND = 1000000000;

  long long delayNs = delayInMs * NS_PER_MS;
  int seconds = delayNs / NS_PER_SECOND;
  int nanoseconds = delayNs % NS_PER_SECOND;

  struct timespec reqDelay = {seconds, nanoseconds};
  nanosleep(&reqDelay, (struct timespec *)NULL);
}

/*
Following Code taken from Dr. Brian Fraser's I2C Guide, pages 10,11
*/
int initI2cBus(char *bus, int address) {
  int i2cFileDesc = open(bus, O_RDWR);
  int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
  if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address");
    exit(1);
  }
  return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
  unsigned char buff[2];
  buff[0] = regAddr;
  buff[1] = value;
  int res = write(i2cFileDesc, buff, 2);
  if (res != 2) {
    perror("I2C: Unable to write i2c register.");
    exit(1);
  }
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr) {
  int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
  if (res != sizeof(regAddr)) {
    perror("I2C: Unable to read to i2c register.");
    exit(1);
  }

  char value = 0;
  res = read(i2cFileDesc, &value, sizeof(value));
  if (res != sizeof(value)) {
    perror("I2C: Unable to read from i2c register");
    exit(1);
  }
  return value;
}


//echo to file for editing their values
void EchoToFile(char* filePath, char* contents){
	// Open direction file
	int file = open(filePath, O_WRONLY);
	if (file < 0) {
		printf("Failed to read file %s", filePath);
		return;
	}

	write(file, contents, strlen(contents));
	// Close
	close(file);

	return;
}

void Change_Terminate(bool status){
	terminate = status;	
}

bool Get_Terminate(){
	return terminate;	

}

void Change_halt(bool status){
  halt = status;
}

bool Get_halt(){
  return halt;
}

void Change_motor_flag(bool status){
  motor_flag = status;
}

bool Get_motor_flag(){
  return motor_flag;
}
