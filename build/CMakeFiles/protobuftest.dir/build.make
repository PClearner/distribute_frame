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
include CMakeFiles/protobuftest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/protobuftest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/protobuftest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/protobuftest.dir/flags.make

CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o: CMakeFiles/protobuftest.dir/flags.make
CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o: ../test/protobuftest.cpp
CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o: CMakeFiles/protobuftest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o -MF CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o.d -o CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o -c /home/star/workspace/distribution_frame/test/protobuftest.cpp

CMakeFiles/protobuftest.dir/test/protobuftest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/protobuftest.dir/test/protobuftest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/test/protobuftest.cpp > CMakeFiles/protobuftest.dir/test/protobuftest.cpp.i

CMakeFiles/protobuftest.dir/test/protobuftest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/protobuftest.dir/test/protobuftest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/test/protobuftest.cpp -o CMakeFiles/protobuftest.dir/test/protobuftest.cpp.s

CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o: CMakeFiles/protobuftest.dir/flags.make
CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o: ../src/rpc/rpcheader.pb.cc
CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o: CMakeFiles/protobuftest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o -MF CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o.d -o CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o -c /home/star/workspace/distribution_frame/src/rpc/rpcheader.pb.cc

CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/rpc/rpcheader.pb.cc > CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.i

CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/rpc/rpcheader.pb.cc -o CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.s

# Object files for target protobuftest
protobuftest_OBJECTS = \
"CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o" \
"CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o"

# External object files for target protobuftest
protobuftest_EXTERNAL_OBJECTS =

../bin/protobuftest: CMakeFiles/protobuftest.dir/test/protobuftest.cpp.o
../bin/protobuftest: CMakeFiles/protobuftest.dir/src/rpc/rpcheader.pb.cc.o
../bin/protobuftest: CMakeFiles/protobuftest.dir/build.make
../bin/protobuftest: /usr/lib/x86_64-linux-gnu/libprotobuf.so
../bin/protobuftest: CMakeFiles/protobuftest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/protobuftest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/protobuftest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/protobuftest.dir/build: ../bin/protobuftest
.PHONY : CMakeFiles/protobuftest.dir/build

CMakeFiles/protobuftest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/protobuftest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/protobuftest.dir/clean

CMakeFiles/protobuftest.dir/depend:
	cd /home/star/workspace/distribution_frame/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/star/workspace/distribution_frame /home/star/workspace/distribution_frame /home/star/workspace/distribution_frame/build /home/star/workspace/distribution_frame/build /home/star/workspace/distribution_frame/build/CMakeFiles/protobuftest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/protobuftest.dir/depend
