#include "organize.h"

#define WINDOW_SIZE 5
#define POLY_DEGREE 2
#define DATA_LEN 5

static float* yawData;
static float* rollData;
static float* pitchData;
static float* distanceStorage;
static int arr_Index = 0;
static float* storage;
static float* gyroDataHold;
static float* gyroDataSmoothed;

static pthread_mutex_t organizeMutex = PTHREAD_MUTEX_INITIALIZER;

static void lock(){
    pthread_mutex_lock(&organizeMutex);
}

static void unlock(){
    pthread_mutex_unlock(&organizeMutex);
}

static pthread_t organizeThreadID;

void *organizer_Thread();

void moving_average_smooth(float* data[]) {
	lock();
	//Actually using moving average
	int sum = 0;
	for (int i = 0; i < DATA_LEN; i++) {
		sum += data[i];
	}
	for (int i = 0; i < DATA_LEN; i++) {
		storage[i] = sum / (float)DATA_LEN; 
	}
	unlock();
}

void Collect_Sample(){	
	lock();
	gyroDataHold = gyro_getData();
	yawData[arr_Index] = gyroDataHold[0];
	rollData[arr_Index] = gyroDataHold[1];
	pitchData[arr_Index] = gyroDataHold[2];
	distanceStorage[arr_Index] = distance_getData();
	arr_Index = arr_Index + 1;
	if(arr_Index < DATA_LEN){		
		arr_Index = 0;
	}
	unlock();
}

void Organize_init(){
	printf("case 1 \n");
	yawData = malloc(10*sizeof(float));
	rollData = malloc(10*sizeof(float));
	pitchData = malloc(10*sizeof(float));
	distanceStorage = malloc(10*sizeof(float));
	storage = malloc(10*sizeof(float));
	//gyroDataHold = malloc(3*sizeof(float));
	gyroDataSmoothed = malloc(3*sizeof(float));
	printf("case 2 \n");
	for(int i = 0; i < DATA_LEN; i++){
		Collect_Sample();
		printf("case %d\n", i);
	}
	Smooth_Data();
	printf("case whatever \n");
	pthread_create(&organizeThreadID, NULL, organizer_Thread,NULL);
}

void Smooth_Data(){	
	moving_average_smooth(yawData);
	for (int i = 0; i < DATA_LEN; i++){
		yawData[i] = storage[i];		
	}
	moving_average_smooth(rollData);
	for (int i = 0; i < DATA_LEN; i++){
		rollData[i] = storage[i];		
	}	
	moving_average_smooth(pitchData);
	for (int i = 0; i < DATA_LEN; i++){
		pitchData[i] = storage[i];		
	}
	moving_average_smooth(distanceStorage);
	for (int i = 0; i < DATA_LEN; i++){
		distanceStorage[i] = storage[i];		
	}
}

float* get_smoothed_gyroData(){
	gyroDataSmoothed[0] = yawData[arr_Index];
	gyroDataSmoothed[1] = rollData[arr_Index];
	gyroDataSmoothed[2] = pitchData[arr_Index];
	return gyroDataSmoothed;	
}

float get_smoothed_distanceData(){	
	return distanceStorage[arr_Index];	
}

void *organizer_Thread(){
	while(Get_Terminate() != true){		
		//collect 3 samples then smooth
		Collect_Sample();
		Collect_Sample();
		Collect_Sample();
		Smooth_Data();
		
	}	
	return NULL;
}

void organize_cleanup(){
	Change_Terminate(true);
	printf("Organize_cleanup Initiated\n");
	pthread_join(organizeThreadID,NULL);
	sleepForMs(500);
	free(yawData);
	free(rollData);
	free(pitchData);
	free(distanceStorage);
	free(storage);
	//free(gyroDataHold);
	free(gyroDataSmoothed);
	printf("Organize_cleanup Completed\n");
}
