# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/star/workspace/distribution_frame

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/star/workspace/distribution_frame/build

# Include any dependencies generated for this target.
include CMakeFiles/inittest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/inittest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/inittest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/inittest.dir/flags.make

CMakeFiles/inittest.dir/src/tools/lock.cpp.o: CMakeFiles/inittest.dir/flags.make
CMakeFiles/inittest.dir/src/tools/lock.cpp.o: ../src/tools/lock.cpp
CMakeFiles/inittest.dir/src/tools/lock.cpp.o: CMakeFiles/inittest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/inittest.dir/src/tools/lock.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/inittest.dir/src/tools/lock.cpp.o -MF CMakeFiles/inittest.dir/src/tools/lock.cpp.o.d -o CMakeFiles/inittest.dir/src/tools/lock.cpp.o -c /home/star/workspace/distribution_frame/src/tools/lock.cpp

CMakeFiles/inittest.dir/src/tools/lock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/inittest.dir/src/tools/lock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/tools/lock.cpp > CMakeFiles/inittest.dir/src/tools/lock.cpp.i

CMakeFiles/inittest.dir/src/tools/lock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/inittest.dir/src/tools/lock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/tools/lock.cpp -o CMakeFiles/inittest.dir/src/tools/lock.cpp.s

CMakeFiles/inittest.dir/src/tools/logger.cpp.o: CMakeFiles/inittest.dir/flags.make
CMakeFiles/inittest.dir/src/tools/logger.cpp.o: ../src/tools/logger.cpp
CMakeFiles/inittest.dir/src/tools/logger.cpp.o: CMakeFiles/inittest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/inittest.dir/src/tools/logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/inittest.dir/src/tools/logger.cpp.o -MF CMakeFiles/inittest.dir/src/tools/logger.cpp.o.d -o CMakeFiles/inittest.dir/src/tools/logger.cpp.o -c /home/star/workspace/distribution_frame/src/tools/logger.cpp

CMakeFiles/inittest.dir/src/tools/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/inittest.dir/src/tools/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/tools/logger.cpp > CMakeFiles/inittest.dir/src/tools/logger.cpp.i

CMakeFiles/inittest.dir/src/tools/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/inittest.dir/src/tools/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/tools/logger.cpp -o CMakeFiles/inittest.dir/src/tools/logger.cpp.s

CMakeFiles/inittest.dir/src/rpc/init.cpp.o: CMakeFiles/inittest.dir/flags.make
CMakeFiles/inittest.dir/src/rpc/init.cpp.o: ../src/rpc/init.cpp
CMakeFiles/inittest.dir/src/rpc/init.cpp.o: CMakeFiles/inittest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/inittest.dir/src/rpc/init.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/inittest.dir/src/rpc/init.cpp.o -MF CMakeFiles/inittest.dir/src/rpc/init.cpp.o.d -o CMakeFiles/inittest.dir/src/rpc/init.cpp.o -c /home/star/workspace/distribution_frame/src/rpc/init.cpp

CMakeFiles/inittest.dir/src/rpc/init.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/inittest.dir/src/rpc/init.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/rpc/init.cpp > CMakeFiles/inittest.dir/src/rpc/init.cpp.i

CMakeFiles/inittest.dir/src/rpc/init.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/inittest.dir/src/rpc/init.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/rpc/init.cpp -o CMakeFiles/inittest.dir/src/rpc/init.cpp.s

CMakeFiles/inittest.dir/test/inittest.cpp.o: CMakeFiles/inittest.dir/flags.make
CMakeFiles/inittest.dir/test/inittest.cpp.o: ../test/inittest.cpp
CMakeFiles/inittest.dir/test/inittest.cpp.o: CMakeFiles/inittest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/inittest.dir/test/inittest.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/inittest.dir/test/inittest.cpp.o -MF CMakeFiles/inittest.dir/test/inittest.cpp.o.d -o CMakeFiles/inittest.dir/test/inittest.cpp.o -c /home/star/workspace/distribution_frame/test/inittest.cpp

CMakeFiles/inittest.dir/test/inittest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/inittest.dir/test/inittest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/test/inittest.cpp > CMakeFiles/inittest.dir/test/inittest.cpp.i

CMakeFiles/inittest.dir/test/inittest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/inittest.dir/test/inittest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/test/inittest.cpp -o CMakeFiles/inittest.dir/test/inittest.cpp.s

# Object files for target inittest
inittest_OBJECTS = \
"CMakeFiles/inittest.dir/src/tools/lock.cpp.o" \
"CMakeFiles/inittest.dir/src/tools/logger.cpp.o" \
"CMakeFiles/inittest.dir/src/rpc/init.cpp.o" \
"CMakeFiles/inittest.dir/test/inittest.cpp.o"

# External object files for target inittest
inittest_EXTERNAL_OBJECTS =

../bin/inittest: CMakeFiles/inittest.dir/src/tools/lock.cpp.o
../bin/inittest: CMakeFiles/inittest.dir/src/tools/logger.cpp.o
../bin/inittest: CMakeFiles/inittest.dir/src/rpc/init.cpp.o
../bin/inittest: CMakeFiles/inittest.dir/test/inittest.cpp.o
../bin/inittest: CMakeFiles/inittest.dir/build.make
../bin/inittest: ../lib/libyaml-cpp.a
../bin/inittest: CMakeFiles/inittest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ../bin/inittest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/inittest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/inittest.dir/build: ../bin/inittest
.PHONY : CMakeFiles/inittest.dir/build

CMakeFiles/inittest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/inittest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/inittest.dir/clean

CMakeFiles/inittest.dir/depend:
	cd /home/star/workspace/distribution_frame/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/star/workspace/distribution_frame /home/star/workspace/distribution_frame /home/star/workspace/distribution_frame/build /home/star/workspace/distribution_frame/build /home/star/workspace/distribution_frame/build/CMakeFiles/inittest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/inittest.dir/depend
