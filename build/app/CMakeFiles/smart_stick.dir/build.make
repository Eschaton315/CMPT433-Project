# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ianh/cmpt433/work/group/CMPT433-Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ianh/cmpt433/work/group/CMPT433-Project/build

# Include any dependencies generated for this target.
include app/CMakeFiles/smart_stick.dir/depend.make

# Include the progress variables for this target.
include app/CMakeFiles/smart_stick.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/smart_stick.dir/flags.make

app/CMakeFiles/smart_stick.dir/src/foo.c.o: app/CMakeFiles/smart_stick.dir/flags.make
app/CMakeFiles/smart_stick.dir/src/foo.c.o: ../app/src/foo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ianh/cmpt433/work/group/CMPT433-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object app/CMakeFiles/smart_stick.dir/src/foo.c.o"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/smart_stick.dir/src/foo.c.o -c /home/ianh/cmpt433/work/group/CMPT433-Project/app/src/foo.c

app/CMakeFiles/smart_stick.dir/src/foo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/smart_stick.dir/src/foo.c.i"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ianh/cmpt433/work/group/CMPT433-Project/app/src/foo.c > CMakeFiles/smart_stick.dir/src/foo.c.i

app/CMakeFiles/smart_stick.dir/src/foo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/smart_stick.dir/src/foo.c.s"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ianh/cmpt433/work/group/CMPT433-Project/app/src/foo.c -o CMakeFiles/smart_stick.dir/src/foo.c.s

app/CMakeFiles/smart_stick.dir/src/main.c.o: app/CMakeFiles/smart_stick.dir/flags.make
app/CMakeFiles/smart_stick.dir/src/main.c.o: ../app/src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ianh/cmpt433/work/group/CMPT433-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object app/CMakeFiles/smart_stick.dir/src/main.c.o"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/smart_stick.dir/src/main.c.o -c /home/ianh/cmpt433/work/group/CMPT433-Project/app/src/main.c

app/CMakeFiles/smart_stick.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/smart_stick.dir/src/main.c.i"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ianh/cmpt433/work/group/CMPT433-Project/app/src/main.c > CMakeFiles/smart_stick.dir/src/main.c.i

app/CMakeFiles/smart_stick.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/smart_stick.dir/src/main.c.s"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && /usr/bin/arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ianh/cmpt433/work/group/CMPT433-Project/app/src/main.c -o CMakeFiles/smart_stick.dir/src/main.c.s

# Object files for target smart_stick
smart_stick_OBJECTS = \
"CMakeFiles/smart_stick.dir/src/foo.c.o" \
"CMakeFiles/smart_stick.dir/src/main.c.o"

# External object files for target smart_stick
smart_stick_EXTERNAL_OBJECTS =

app/smart_stick: app/CMakeFiles/smart_stick.dir/src/foo.c.o
app/smart_stick: app/CMakeFiles/smart_stick.dir/src/main.c.o
app/smart_stick: app/CMakeFiles/smart_stick.dir/build.make
app/smart_stick: hal/libhal.a
app/smart_stick: hal/libhal.a
app/smart_stick: app/CMakeFiles/smart_stick.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ianh/cmpt433/work/group/CMPT433-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable smart_stick"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/smart_stick.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copying ARM executable to public NFS directory"
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && /usr/bin/cmake -E copy /home/ianh/cmpt433/work/group/CMPT433-Project/build/app/smart_stick ~/cmpt433/public/myApps/smart_stick

# Rule to build all files generated by this target.
app/CMakeFiles/smart_stick.dir/build: app/smart_stick

.PHONY : app/CMakeFiles/smart_stick.dir/build

app/CMakeFiles/smart_stick.dir/clean:
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build/app && $(CMAKE_COMMAND) -P CMakeFiles/smart_stick.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/smart_stick.dir/clean

app/CMakeFiles/smart_stick.dir/depend:
	cd /home/ianh/cmpt433/work/group/CMPT433-Project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ianh/cmpt433/work/group/CMPT433-Project /home/ianh/cmpt433/work/group/CMPT433-Project/app /home/ianh/cmpt433/work/group/CMPT433-Project/build /home/ianh/cmpt433/work/group/CMPT433-Project/build/app /home/ianh/cmpt433/work/group/CMPT433-Project/build/app/CMakeFiles/smart_stick.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/CMakeFiles/smart_stick.dir/depend

