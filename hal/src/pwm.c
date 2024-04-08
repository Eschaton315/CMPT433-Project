#include <stdio.h>
#include <stdlib.h>
#include <math.h>  
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "hal/shared.h"
#include "hal/pwm.h"

#define CONFIG_PIN "sudo config-pin P9_22 pwm"

#define PERIOD "/dev/bone/pwm/0/a/period"

#define ENABLE "/dev/bone/pwm/0/a/enable"

#define DUTY_CYCLE "/dev/bone/pwm/0/a/duty_cycle"

#define BUFFER_SIZE 20


//configure pin to Buzzer
void configBuzzer(void){
	runCommand(CONFIG_PIN);
}

//Change duty_cycle to value
void ChangeDutyCycle(int value){
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "%d", value);
	//itoa(value, buffer, 10);
    EchoToFile(DUTY_CYCLE, buffer);
}

//Change period to value
void ChangePeriod(int value){
	char buffer[BUFFER_SIZE];
	sprintf(buffer, "%d", value);
	//itoa(value, buffer, 10);
    EchoToFile(PERIOD, buffer);
}

//Turn buzzer on by changing enable to 1
void TurnBuzzerOn(){
    EchoToFile(ENABLE, "1");
} 

//Turn buzzer off by changing enable to 0
void TurnBuzzerOff(){
    EchoToFile(ENABLE, "0");
} 

