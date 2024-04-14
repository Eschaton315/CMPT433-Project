# CMPT 433 Smart Stick Structure

```
.
├── app
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── foo.h
│   │   ├── organize.h
│   │   └── pwmDriver.h
│   └── src
│       ├── foo.c
│       ├── main.c
│       ├── organize.c
│       └── pwmDriver.c
├── CMakeLists.txt
├── hal
│   ├── CMakeLists.txt
│   ├── include
│   │   └── hal
│   │       ├── distanceSensor.h
│   │       ├── gyroscope.h
│   │       ├── I2Cdev.h
│   │       ├── joyStick.h
│   │       ├── motor.h
│   │       ├── MPU6050.h
│   │       ├── pwm.h
│   │       └── shared.h
│   └── src
│       ├── bno085Scrap
│       ├── distanceSensor.c
│       ├── gyroscope.c
│       ├── I2Cdev.c
│       ├── joyStick.c
│       ├── motor.c
│       ├── MPU6050.c
│       ├── pwm.c
│       └── shared.c
├── lib
│   └── VL53L0X
│       ├── libtof.a
│       ├── LICENSE.txt
│       ├── main.c
│       ├── make_demo
│       ├── Makefile
│       ├── README.md
│       ├── tof.c
│       ├── tof.h
│       └── tof.o
└── README.md


```