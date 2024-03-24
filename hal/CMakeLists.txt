# CMakeList.txt for HAL
#   Build a library (`hal`) which exposes the header files as "hal/*.h"
#   Use header as: #include "hal/button.h"

include_directories(hal/include)
include_directories(hal/include/BNO085_gyro)
include_directories(/usr/local/include)

file(GLOB MY_SOURCES "src/*.c")

add_library(hal STATIC ${MY_SOURCES})

target_include_directories(hal PUBLIC include)
