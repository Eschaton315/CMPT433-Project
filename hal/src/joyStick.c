
#include "hal/joyStick.h"


#define STICK_UP "/sys/class/gpio/gpio26/value"
#define STICK_DOWN "/sys/class/gpio/gpio46/value"
#define STICK_LEFT "/sys/class/gpio/gpio65/value"
#define STICK_RIGHT "/sys/class/gpio/gpio47/value"
#define STICK_IN "/sys/class/gpio/gpio27/value"

int joystickValue = 0;
bool stopListen = false;
pthread_t joystickThread;

// Thread synchronization
static pthread_mutex_t joystickMutex = PTHREAD_MUTEX_INITIALIZER;

static void lock(){
    pthread_mutex_lock(&joystickMutex);
}
static void unlock(){
    pthread_mutex_unlock(&joystickMutex);
}


//configure pin to GPIO
void configPinGPIO(){
runCommand("config-pin p8.14 gpio");
runCommand("config-pin p8.15 gpio");
runCommand("config-pin p8.16 gpio");
runCommand("config-pin p8.18 gpio");
runCommand("config-pin p8.17 gpio");
}


static void *joystickListener();

void joystick_init(){
    configPinGPIO();
    pthread_create(&joystickThread, NULL, &joystickListener,NULL);
    return;
}

//Boolean of whether a joystick of designated path is pressed

bool joystickPressed(char *path){

    FILE *pFile = fopen(path, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", path);
        return false;
    }
    // Read string (line)

    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);

    int value = atoi(buff);

    if(value==0){
            return true;
    }else{
            return false;
    }


}

//sets which direction the joystick is pressed

void setJoystickValue(){
    lock();
    if(joystickPressed(STICK_IN)){
        joystickValue = 5;
    }else if(joystickPressed(STICK_UP)){
        joystickValue = 1;
    }else if(joystickPressed(STICK_DOWN)){
        joystickValue = 2;
    }else if(joystickPressed(STICK_LEFT)){
        joystickValue = 3;
    }else if(joystickPressed(STICK_RIGHT)){
        joystickValue = 4;
    }else{
        joystickValue = 0;
    }
    unlock();
}


static void *joystickListener(){
    while(!stopListen){
        setJoystickValue();
    }
    return NULL;
}



void joystickListener_cleanup(){
    stopListen = true;
    pthread_join(joystickThread,NULL);

    return;
}


//returns which direction the joystick is pressed
int joystick_getJoystickValue(){
    return joystickValue;
}

