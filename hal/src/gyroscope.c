#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "hal/shared.h"

#define I2C_PATH "/dev/i2c-2"
#define addr 0x4A

#define BUFFER_SIZE 128


void gyro_init(void) {
    runCommand("config-pin p9.19 i2c");
    runCommand("config-pin p9.20 i2c");
}

void gyro_readData(){

   // unsigned int buf[3];
    int file;

 unsigned char buf[2];  // Buffer to hold register address and command
   unsigned char data[6];  // Buffer to hold gyroscope data (3 axes * 2 bytes each)

        if ((file = open(I2C_PATH, O_RDWR)) < 0) {
        perror("Failed to open the bus.");
        exit(1);
    }

    // Set I2C device address
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("Failed to acquire bus access and/or talk to slave.");
        close(file);
        exit(1);
    }

// Send command to request gyroscope data (starting at register 0x0E)

    buf[0] = 0x5E;  // Command byte
    buf[1] = 0x0E;  // Register address for gyroscope data
    if (write(file, buf, 2) != 2) {
        perror("Failed to write to I2C bus.");
        close(file);
        exit(1);
    }

    // Read gyroscope data (6 bytes for X, Y, Z axes)
    if (read(file, data, 6) != 6) {
        perror("Failed to read from I2C bus.");
        close(file);
        exit(1);
    }

    // Close I2C bus
    close(file);

    

    // Process and use gyroscope data
    int gyro_x = (data[1] << 8) | data[0];  // Combine bytes for X-axis
   int gyro_y = (data[3] << 8) | data[2];  // Combine bytes for Y-axis
    int gyro_z = (data[5] << 8) | data[4];  // Combine bytes for Z-axis


    // Print gyroscope readings
    printf("Gyroscope readings: X=%d, Y=%d, Z=%d\n", gyro_x, gyro_y, gyro_z);
     printf("Raw Gyroscope Data (Hexadecimal):\n");
    for (int i = 0; i < 6; i++) {
        printf("%02X ", data[i]);  // Print each byte in hexadecimal format
    }
    printf("\n");

    // Close I2C bus
    close(file);
}
