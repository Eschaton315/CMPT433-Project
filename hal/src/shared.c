#include "hal/shared.h"
#include <string.h>

static bool terminate = false;
static bool halt = false;
static bool motor_flag = false;


// Runs provided termminal command
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

// Returns system real time clock in nanoseconds
long long getTimeInMs()
{
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  long long seconds = spec.tv_sec;
  long long nanoSeconds = spec.tv_nsec;
  long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
  return milliSeconds;
}

// Sleeps the calling thread for specified amount of milliseconds
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
//Sets specified I2C device address to slave address 
int initI2cBus(char *bus, int address) {
  int i2cFileDesc = open(bus, O_RDWR);
  int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
  if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address");
    exit(1);
  }
  return i2cFileDesc;
}

// Writes the value into the designated i2c device register address 
// Must provide a valid i2cFileDescriptor and valid register address
// Code taken from Dr. Brian Fraser's I2C guide
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

// Returns contents of the designated I2C device register address
// Code taken from Dr. Brian Fraser's I2C guide
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

// Writes contents into the designated file
// Mimics the functionality of "echo" command
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

//Changes the terminate flag to status
void Change_Terminate(bool status){
	terminate = status;	
}

// Returns the value of terminate flag
bool Get_Terminate(){
	return terminate;	

}

// Changes the halt flag to status
void Change_halt(bool status){
  halt = status;
}

// Returns the value of halt flag
bool Get_halt(){
  return halt;
}

// Changes the value of motor flag to status
// Used by the motor module to initiate driving the vibration motor
void Change_motor_flag(bool status){
  motor_flag = status;
}

// Returns the value of motor flag
bool Get_motor_flag(){
  return motor_flag;
}
