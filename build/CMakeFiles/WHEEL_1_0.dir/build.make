# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yz/wheel

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yz/wheel/build

# Include any dependencies generated for this target.
include CMakeFiles/WHEEL_1_0.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/WHEEL_1_0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/WHEEL_1_0.dir/flags.make

CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o: CMakeFiles/WHEEL_1_0.dir/flags.make
CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o: ../src/lxIPC.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o   -c /home/yz/wheel/src/lxIPC.c

CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yz/wheel/src/lxIPC.c > CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.i

CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yz/wheel/src/lxIPC.c -o CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.s

CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.requires:

.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.requires

CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.provides: CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.requires
	$(MAKE) -f CMakeFiles/WHEEL_1_0.dir/build.make CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.provides.build
.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.provides

CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.provides.build: CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o


CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o: CMakeFiles/WHEEL_1_0.dir/flags.make
CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o: ../src/lxMemoryPool.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o   -c /home/yz/wheel/src/lxMemoryPool.c

CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yz/wheel/src/lxMemoryPool.c > CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.i

CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yz/wheel/src/lxMemoryPool.c -o CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.s

CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.requires:

.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.requires

CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.provides: CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.requires
	$(MAKE) -f CMakeFiles/WHEEL_1_0.dir/build.make CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.provides.build
.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.provides

CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.provides.build: CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o


CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o: CMakeFiles/WHEEL_1_0.dir/flags.make
CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o: ../src/lxQue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o   -c /home/yz/wheel/src/lxQue.c

CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yz/wheel/src/lxQue.c > CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.i

CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yz/wheel/src/lxQue.c -o CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.s

CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.requires:

.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.requires

CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.provides: CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.requires
	$(MAKE) -f CMakeFiles/WHEEL_1_0.dir/build.make CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.provides.build
.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.provides

CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.provides.build: CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o


CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o: CMakeFiles/WHEEL_1_0.dir/flags.make
CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o: ../src/lxSocket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o   -c /home/yz/wheel/src/lxSocket.c

CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yz/wheel/src/lxSocket.c > CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.i

CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yz/wheel/src/lxSocket.c -o CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.s

CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.requires:

.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.requires

CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.provides: CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.requires
	$(MAKE) -f CMakeFiles/WHEEL_1_0.dir/build.make CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.provides.build
.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.provides

CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.provides.build: CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o


CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o: CMakeFiles/WHEEL_1_0.dir/flags.make
CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o: ../src/lxMsgQue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o   -c /home/yz/wheel/src/lxMsgQue.c

CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yz/wheel/src/lxMsgQue.c > CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.i

CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yz/wheel/src/lxMsgQue.c -o CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.s

CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.requires:

.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.requires

CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.provides: CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.requires
	$(MAKE) -f CMakeFiles/WHEEL_1_0.dir/build.make CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.provides.build
.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.provides

CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.provides.build: CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o


CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o: CMakeFiles/WHEEL_1_0.dir/flags.make
CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o: ../src/lxOs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o   -c /home/yz/wheel/src/lxOs.c

CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yz/wheel/src/lxOs.c > CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.i

CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yz/wheel/src/lxOs.c -o CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.s

CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.requires:

.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.requires

CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.provides: CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.requires
	$(MAKE) -f CMakeFiles/WHEEL_1_0.dir/build.make CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.provides.build
.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.provides

CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.provides.build: CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o


CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o: CMakeFiles/WHEEL_1_0.dir/flags.make
CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o: ../src/lxList.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o   -c /home/yz/wheel/src/lxList.c

CMakeFiles/WHEEL_1_0.dir/src/lxList.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WHEEL_1_0.dir/src/lxList.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yz/wheel/src/lxList.c > CMakeFiles/WHEEL_1_0.dir/src/lxList.c.i

CMakeFiles/WHEEL_1_0.dir/src/lxList.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WHEEL_1_0.dir/src/lxList.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yz/wheel/src/lxList.c -o CMakeFiles/WHEEL_1_0.dir/src/lxList.c.s

CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.requires:

.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.requires

CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.provides: CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.requires
	$(MAKE) -f CMakeFiles/WHEEL_1_0.dir/build.make CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.provides.build
.PHONY : CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.provides

CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.provides.build: CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o


# Object files for target WHEEL_1_0
WHEEL_1_0_OBJECTS = \
"CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o" \
"CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o" \
"CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o" \
"CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o" \
"CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o" \
"CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o" \
"CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o"

# External object files for target WHEEL_1_0
WHEEL_1_0_EXTERNAL_OBJECTS =

libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/build.make
libWHEEL_1_0.so: CMakeFiles/WHEEL_1_0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yz/wheel/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C shared library libWHEEL_1_0.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WHEEL_1_0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/WHEEL_1_0.dir/build: libWHEEL_1_0.so

.PHONY : CMakeFiles/WHEEL_1_0.dir/build

CMakeFiles/WHEEL_1_0.dir/requires: CMakeFiles/WHEEL_1_0.dir/src/lxIPC.c.o.requires
CMakeFiles/WHEEL_1_0.dir/requires: CMakeFiles/WHEEL_1_0.dir/src/lxMemoryPool.c.o.requires
CMakeFiles/WHEEL_1_0.dir/requires: CMakeFiles/WHEEL_1_0.dir/src/lxQue.c.o.requires
CMakeFiles/WHEEL_1_0.dir/requires: CMakeFiles/WHEEL_1_0.dir/src/lxSocket.c.o.requires
CMakeFiles/WHEEL_1_0.dir/requires: CMakeFiles/WHEEL_1_0.dir/src/lxMsgQue.c.o.requires
CMakeFiles/WHEEL_1_0.dir/requires: CMakeFiles/WHEEL_1_0.dir/src/lxOs.c.o.requires
CMakeFiles/WHEEL_1_0.dir/requires: CMakeFiles/WHEEL_1_0.dir/src/lxList.c.o.requires

.PHONY : CMakeFiles/WHEEL_1_0.dir/requires

CMakeFiles/WHEEL_1_0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/WHEEL_1_0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/WHEEL_1_0.dir/clean

CMakeFiles/WHEEL_1_0.dir/depend:
	cd /home/yz/wheel/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yz/wheel /home/yz/wheel /home/yz/wheel/build /home/yz/wheel/build /home/yz/wheel/build/CMakeFiles/WHEEL_1_0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/WHEEL_1_0.dir/depend

