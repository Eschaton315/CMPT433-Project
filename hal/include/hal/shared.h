#ifndef _SHARED_H_
#define _SHARED_H_

#include <assert.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

void writeCmd(char *, char *);
void runCommand(char *command);
long long getTimeInMs(void);
void sleepForMs(long long delayInMs);
int initI2cBus(char *, int);
void writeI2cReg(int, unsigned char, unsigned char);
unsigned char readI2cReg(int, unsigned char);
void writeToFile(FILE *file, char *value);
void EchoToFile(char* filePath, char* contents);
#endif