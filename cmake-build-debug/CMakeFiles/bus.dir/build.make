# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/antoni4040/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/193.5233.103/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/antoni4040/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/193.5233.103/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/antoni4040/CLionProjects/OS-Project-3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/antoni4040/CLionProjects/OS-Project-3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/bus.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bus.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bus.dir/flags.make

CMakeFiles/bus.dir/src/bus.c.o: CMakeFiles/bus.dir/flags.make
CMakeFiles/bus.dir/src/bus.c.o: ../src/bus.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/antoni4040/CLionProjects/OS-Project-3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bus.dir/src/bus.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bus.dir/src/bus.c.o   -c /home/antoni4040/CLionProjects/OS-Project-3/src/bus.c

CMakeFiles/bus.dir/src/bus.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bus.dir/src/bus.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/antoni4040/CLionProjects/OS-Project-3/src/bus.c > CMakeFiles/bus.dir/src/bus.c.i

CMakeFiles/bus.dir/src/bus.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bus.dir/src/bus.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/antoni4040/CLionProjects/OS-Project-3/src/bus.c -o CMakeFiles/bus.dir/src/bus.c.s

# Object files for target bus
bus_OBJECTS = \
"CMakeFiles/bus.dir/src/bus.c.o"

# External object files for target bus
bus_EXTERNAL_OBJECTS =

bus: CMakeFiles/bus.dir/src/bus.c.o
bus: CMakeFiles/bus.dir/build.make
bus: CMakeFiles/bus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/antoni4040/CLionProjects/OS-Project-3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bus"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bus.dir/build: bus

.PHONY : CMakeFiles/bus.dir/build

CMakeFiles/bus.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bus.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bus.dir/clean

CMakeFiles/bus.dir/depend:
	cd /home/antoni4040/CLionProjects/OS-Project-3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antoni4040/CLionProjects/OS-Project-3 /home/antoni4040/CLionProjects/OS-Project-3 /home/antoni4040/CLionProjects/OS-Project-3/cmake-build-debug /home/antoni4040/CLionProjects/OS-Project-3/cmake-build-debug /home/antoni4040/CLionProjects/OS-Project-3/cmake-build-debug/CMakeFiles/bus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bus.dir/depend

