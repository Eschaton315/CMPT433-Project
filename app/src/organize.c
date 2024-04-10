#include "organize.h"

#define WINDOW_SIZE 5
#define POLY_DEGREE 2
#define DATA_LEN 10

static float* yawData;
static float* rollData;
static float* pitchData;
static float* distanceStorage;
static int index = 0;
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

void *organizer_Thread(void *);

static float sg_coefficients[] = {-3, 12, 17, 12, -3};

void savitzky_golay_smooth(float data[]) {
	lock();
    int i, j;
    float smoothed_data[DATA_LEN];

    // Iterate over each data point
    for (i = 2; i < DATA_LEN - 2; i++) {
        float sum = 0.0;
        // Apply convolution with coefficients
        for (j = -2; j <= 2; j++) {
            sum += sg_coefficients[j + 2] * data[i + j];
        }
        // Store the smoothed value
        smoothed_data[i] = sum / 35.0; // Sum of coefficients is 35
    }
	unlock();
	
    for(int k = 0; k < 10; k++){
		storage[k] = smoothed_data[k];		
	}
}

void Collect_Sample(){	
	lock();
	gyroDataHold = gyro_getData();
	yawData[index] = gyroDataHold[0];
	rollData[index] = gyroDataHold[1];
	pitchData[index] = gyroDataHold[2];
	distanceStorage[index] = distance_getData();
	index = index + 1;
	if(index == 10){		
		index = 0;
	}
	unlock();
}

void Organize_init(){
	yawData = malloc(10*sizeof(float));
	rollData = malloc(10*sizeof(float));
	pitchData = malloc(10*sizeof(float));
	distanceStorage = malloc(10*sizeof(float));
	storage = malloc(10*sizeof(float));
	gyroDataHold = malloc(3*sizeof(float));
	gyroDataSmoothed = malloc(3*sizeof(float));
	
	for(int i = 0; i < 10; i++){
		Collect_Sample();
		
	}
	Smooth_Data();
	pthread_create(&organizeThreadID, NULL, organizer_Thread,NULL);
}

void Smooth_Data(){	
	savitzky_golay_smooth(yawData);
	yawData = storage;
	savitzky_golay_smooth(rollData);
	rollData = storage;
	savitzky_golay_smooth(pitchData);
	pitchData = storage;
	savitzky_golay_smooth(distanceStorage);
	distanceStorage = storage;
	
}

float* get_smoothed_gyroData(){
	gyroDataSmoothed[0] = yawData[index];
	gyroDataSmoothed[1] = rollData[index];
	gyroDataSmoothed[2] = pitchData[index];
	return gyroDataSmoothed;	
}

float get_smoothed_distanceData(){	
	return distanceStorage[index];	
}

void *organizer_Thread(void *arg){
	while(Get_Terminate() != true){		
		//collect 2 samples then smooth
		Collect_Sample();
		Collect_Sample();
		Smooth_Data();
		
	}	
	return NULL;
}

void oraganize_cleanup(){
	printf("Organize_cleanup Initiated\n");
	free(yawData);
	free(rollData);
	free(pitchData);
	free(distanceStorage);
	free(storage);
	free(gyroDataHold);
	free(gyroDataSmoothed);
	pthread_join(organizeThreadID,NULL);
	printf("Organize_cleanup Completed\n");
}
