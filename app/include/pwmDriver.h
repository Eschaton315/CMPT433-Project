#ifndef _PWMDRIVER_H_
#define _PWMDRIVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "hal/pwm.h"
#include "hal/shared.h"

//Creates thread for buzzer on Hit
void BuzzerHitThreadCreate(void);

//Join Thread for buzzer on Hit
void BuzzerHitThreadJoin(void);

//Creates thread for buzzer on Miss
void BuzzerMissThreadCreate(void);

//Join Thread for buzzer on Miss 
void BuzzerMissThreadJoin(void);

#endif