#ifndef _DISTANCE_SENSOR_H_
#define _DISTANCE_SENSOR_H_

#include <stdbool.h>

void button_init(void);
bool button_is_button_pressed(void);
void button_cleanup(void);

#endif