#include "organize.h"

#define WINDOW_SIZE 5
#define POLY_DEGREE 2

static float* gyroDataStorage;
static float* distanceStorage;
static int index = 0;
static int data_length = 10;
static float* gyroDataSmoothed;
static float* gyroDataHold;
static float* storage;

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
    float smoothed_data[data_length];

    // Iterate over each data point
    for (i = 2; i < data_length - 2; i++) {
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


void Collect_Initial_Samples(){
	for (int i = 0; i < 10; i++){
		gyroDataHold = gyro_getData();
		gyroDataStorage[0][i] = gyroDataHold[0];
		gyroDataStorage[1][i] = gyroDataHold[1];
		gyroDataStorage[2][i] = gyroDataHold[2];
		distanceStorage[i] = distance_getData();
	}
}

void Collect_Sample(){	
	lock();
	gyroDataStorage[index] = gyro_getData();
	distanceStorage[index] = distance_getData();
	index = index + 1;
	if(index == 10){		
		index = 0;
	}
	unlock();
}

void Organize_init(){
	Collect_Initial_Samples();
	Smooth_Data();
	gyroDataStorage = malloc(3*10*sizeof(float));
	distanceStorage = malloc(10*sizeof(float));
	gyroDataSmoothed = malloc(3*sizeof(float));
	gyroDataHold = malloc(3*sizeof(float));
	storage = malloc(10*sizeof(float));
	pthread_create(&organizeThreadID, NULL, organizer_Thread,NULL);
}

void Smooth_Data(){	
	savitzky_golay_smooth(gyroDataStorage[0]);
	gyroDataStorage[0] = storage;
	savitzky_golay_smooth(gyroDataStorage[1]);
	gyroDataStorage[1] = storage;
	savitzky_golay_smooth(gyroDataStorage[2]);
	gyroDataStorage[2] = storage;
	savitzky_golay_smooth(distanceStorage);
	distanceStorage = storage;
	
}

float* get_smoothed_gyroData(){
	gyroDataSmoothed[0] = gyroDataStorage[0][index];
	gyroDataSmoothed[1] = gyroDataStorage[1][index];
	gyroDataSmoothed[2] = gyroDataStorage[2][index];
	return gyroDataSmoothed;	
}

float* get_smoothed_distanceData(){	
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
	free(gyroDataStorage);
	free(distanceStorage);
	free(gyroDataSmoothed);
	free(distanceStorage);
	pthread_join(organizeThreadID,NULL);
	printf("Organize_cleanup Completed\n");
}
