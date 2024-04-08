#ifndef _PWM_H_
#define _PWM_H_

void configBuzzer(void);

//Change duty_cycle to value
void ChangeDutyCycle(int value);

//Change period to value
void ChangePeriod(int value);

//Turn buzzer on by changing enable to 0
void TurnBuzzerOn();

//Turn buzzer off by changing enable to 1
void TurnBuzzerOff();

#endif