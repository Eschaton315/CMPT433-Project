#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "hal/shared.h"

#define I2C_PATH "/dev/i2c-2"
#define addr 0x4A

#define READ_SIZE 16


void gyro_init(void) {
    runCommand("config-pin p9.19 i2c");
    runCommand("config-pin p9.20 i2c");
}

void gyro_readData(){

    int file;
    bool mode = false;
   char buf[2];  
   char data[READ_SIZE];  

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

    if(mode){
        //ONLY PRINTS OUT 00's
        size_t bytesRead = read(file, data,sizeof(data));

                if (bytesRead > 0) {
                printf("Read %zu bytes in hexadecimal format:\n", bytesRead);
                for (size_t i = 0; i < bytesRead; i++) {
                    printf("%02X ", data[i]); // Print each byte in hexadecimal format
                }
                printf("\n");
            } else {
                printf("Error reading file.\n");
            }
    }else{

        buf[0] = 0x02;  // Command byte according to the documentation, should let me access the gyro data.
        buf[1] = 0x0E;
        // I dont know what i am supposed to write for the second byte since the documentation only says "reserved"
        if (write(file, buf, 2) != 2) {
            perror("Failed to write to I2C bus.");
            close(file);
            exit(1);
        }



        if (read(file, data, READ_SIZE) != READ_SIZE) {
            perror("Failed to read from I2C bus.");
            close(file);
            exit(1);
        }

        // Close I2C bus
        close(file);
        printf("Raw Gyroscope Data? (Hexadecimal):\n");
        for (int i = 0; i < READ_SIZE; i++) {
            printf("%02X ", data[i]);  // Print each byte in hexadecimal format
        }
        printf("\n");
    }
}
