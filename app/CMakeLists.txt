# Build the app, using the HAL

include_directories(include)
include_directories(/usr/local/include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(smart_stick ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(smart_stick LINK_PRIVATE hal)
target_link_libraries(smart_stick LINK_PRIVATE hal tof)

# Copy executable to final location
add_custom_command(TARGET smart_stick POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:smart_stick>"
     "~/cmpt433/public/myApps/smart_stick" 
  COMMENT "Copying ARM executable to public NFS directory")
