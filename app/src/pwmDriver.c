#include "pwmDriver.h"

pthread_t BuzzerHitThread;
pthread_t BuzzerMissThread;

static void* BuzzerHit();
static void* BuzzerMiss();

static int hitPeriod = 1000000;
static int hitDutyCycle = 500000;
static int MissPeriod = 4000000;
static int MissDutyCycle = 1000000;



//Creates thread for buzzer on Hit
void BuzzerHitThreadCreate(void){
	pthread_create(&BuzzerHitThread, NULL, &BuzzerHit, NULL);
}

//Join Thread for buzzer on Hit
void BuzzerHitThreadJoin(void){
	pthread_join(BuzzerHitThread, NULL);
}

//Creates thread for buzzer on Miss
void BuzzerMissThreadCreate(void){
	pthread_create(&BuzzerMissThread, NULL, &BuzzerMiss, NULL);
}

//Join Thread for buzzer on Miss 
void BuzzerMissThreadJoin(void){
	pthread_join(BuzzerMissThread, NULL);
}





static void* BuzzerHit(){
	ChangePeriod(hitPeriod);
	ChangeDutyCycle(hitDutyCycle);
	TurnBuzzerOn();
	sleepForMs(500);
	TurnBuzzerOff();
	return NULL;
}

static void* BuzzerMiss(){
	ChangePeriod(MissPeriod);
	ChangeDutyCycle(MissDutyCycle);
	TurnBuzzerOn();
	sleepForMs(500);
	TurnBuzzerOff();
	return NULL;
}