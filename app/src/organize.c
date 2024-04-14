#include "organize.h"

#define WINDOW_SIZE 5
#define POLY_DEGREE 2
#define DATA_LEN 3

static float* yawData;
static float* rollData;
static float* pitchData;
static float* distanceStorage;
static int arr_Index = 0;
static float* storage;
static float* gyroDataHold;
static float* gyroDataSmoothed;

static pthread_mutex_t organizeMutex = PTHREAD_MUTEX_INITIALIZER;

/*
	File used to organize data from the distant sensor and gyroscope.
	The data is smoothed using the moving average algorithm.
	The thread will save number of data (determined by DATA_LEN).
	Afterward, we will calculate an average. 
	The data from this file will be used as the sample values for main.
*/


static void lock(){
    pthread_mutex_lock(&organizeMutex);
}

static void unlock(){
    pthread_mutex_unlock(&organizeMutex);
}

static pthread_t organizeThreadID;

void *organizer_Thread();

//Use moving average to smooth the array
void moving_average_smooth(float data[]) {
	lock();
	int sum = 0;
	for (int i = 0; i < DATA_LEN; i++) {
		sum += data[i];
	}
	for (int i = 0; i < DATA_LEN; i++) {
		storage[i] = sum / (float)DATA_LEN; 
	}
	unlock();
}

//Collect a sample of data from gyroscope of Yaw, Roll, and Pitch
//Along with a sample of data from the distance sensor and saves it
void Collect_Sample(){	
	lock();
	gyroDataHold = gyro_getData();
	yawData[arr_Index] = gyroDataHold[0];
	rollData[arr_Index] = gyroDataHold[1];
	pitchData[arr_Index] = gyroDataHold[2];
	distanceStorage[arr_Index] = distance_getData();
	arr_Index = (arr_Index + 1) % DATA_LEN;
	unlock();
}

//The initializing function for running the data organizer
void Organize_init(){
	yawData = malloc(10*sizeof(float));
	rollData = malloc(10*sizeof(float));
	pitchData = malloc(10*sizeof(float));
	distanceStorage = malloc(10*sizeof(float));
	storage = malloc(10*sizeof(float));
	gyroDataSmoothed = malloc(3*sizeof(float));
	for(int i = 0; i < DATA_LEN; i++){
		Collect_Sample();
		sleepForMs(10);
	}	
	Smooth_Data();

	pthread_create(&organizeThreadID, NULL, organizer_Thread,NULL);
}

//call the smoothing algorithm on the data and store them
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

//Get the smoothed gyroscope data for use
float* get_smoothed_gyroData(){
	gyroDataSmoothed[0] = yawData[arr_Index];
	gyroDataSmoothed[1] = rollData[arr_Index];
	gyroDataSmoothed[2] = pitchData[arr_Index];
	return gyroDataSmoothed;	
}

//Get the smoothed distance data for use
float get_smoothed_distanceData(){	
	return distanceStorage[arr_Index];	
}

//The thread will collect samples of data of each type,
//Up to the number which is defined as DATA_LEN
//Then the smoothing algorithm will be run
void *organizer_Thread(){
	while(Get_Terminate() != true){		
		if(!Get_halt()){
		//collect all samples then smooth
		for(int i = 0; i < DATA_LEN; i++){
			Collect_Sample();
			sleepForMs(10);
		}
		
		Smooth_Data();
		sleepForMs(50);
		}
	}	
	return NULL;
}

//Clean up for organize thread
//Must be called at the end
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
