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
include app/rpc/CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include app/rpc/CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include app/rpc/CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include app/rpc/CMakeFiles/client.dir/flags.make

app/rpc/CMakeFiles/client.dir/client.cpp.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/client.cpp.o: ../app/rpc/client.cpp
app/rpc/CMakeFiles/client.dir/client.cpp.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object app/rpc/CMakeFiles/client.dir/client.cpp.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/client.cpp.o -MF CMakeFiles/client.dir/client.cpp.o.d -o CMakeFiles/client.dir/client.cpp.o -c /home/star/workspace/distribution_frame/app/rpc/client.cpp

app/rpc/CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/app/rpc/client.cpp > CMakeFiles/client.dir/client.cpp.i

app/rpc/CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/app/rpc/client.cpp -o CMakeFiles/client.dir/client.cpp.s

app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o: ../src/tools/lock.cpp
app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o -MF CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o.d -o CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o -c /home/star/workspace/distribution_frame/src/tools/lock.cpp

app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/__/__/src/tools/lock.cpp.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/tools/lock.cpp > CMakeFiles/client.dir/__/__/src/tools/lock.cpp.i

app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/__/__/src/tools/lock.cpp.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/tools/lock.cpp -o CMakeFiles/client.dir/__/__/src/tools/lock.cpp.s

app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o: ../src/tools/logger.cpp
app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o -MF CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o.d -o CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o -c /home/star/workspace/distribution_frame/src/tools/logger.cpp

app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/__/__/src/tools/logger.cpp.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/tools/logger.cpp > CMakeFiles/client.dir/__/__/src/tools/logger.cpp.i

app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/__/__/src/tools/logger.cpp.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/tools/logger.cpp -o CMakeFiles/client.dir/__/__/src/tools/logger.cpp.s

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o: ../src/rpc/init.cpp
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o -MF CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o.d -o CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o -c /home/star/workspace/distribution_frame/src/rpc/init.cpp

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/__/__/src/rpc/init.cpp.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/rpc/init.cpp > CMakeFiles/client.dir/__/__/src/rpc/init.cpp.i

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/__/__/src/rpc/init.cpp.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/rpc/init.cpp -o CMakeFiles/client.dir/__/__/src/rpc/init.cpp.s

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o: ../src/rpc/rpcheader.pb.cc
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o -MF CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o.d -o CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o -c /home/star/workspace/distribution_frame/src/rpc/rpcheader.pb.cc

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/rpc/rpcheader.pb.cc > CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.i

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/rpc/rpcheader.pb.cc -o CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.s

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o: ../src/rpc/rpcprovider.cpp
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o -MF CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o.d -o CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o -c /home/star/workspace/distribution_frame/src/rpc/rpcprovider.cpp

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/rpc/rpcprovider.cpp > CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.i

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/rpc/rpcprovider.cpp -o CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.s

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o: ../src/rpc/zookeeperutil.cpp
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o -MF CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o.d -o CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o -c /home/star/workspace/distribution_frame/src/rpc/zookeeperutil.cpp

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/rpc/zookeeperutil.cpp > CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.i

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/rpc/zookeeperutil.cpp -o CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.s

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o: ../src/rpc/rpcchannel.cpp
app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o -MF CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o.d -o CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o -c /home/star/workspace/distribution_frame/src/rpc/rpcchannel.cpp

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/src/rpc/rpcchannel.cpp > CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.i

app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/src/rpc/rpcchannel.cpp -o CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.s

app/rpc/CMakeFiles/client.dir/userservice.pb.cc.o: app/rpc/CMakeFiles/client.dir/flags.make
app/rpc/CMakeFiles/client.dir/userservice.pb.cc.o: ../app/rpc/userservice.pb.cc
app/rpc/CMakeFiles/client.dir/userservice.pb.cc.o: app/rpc/CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object app/rpc/CMakeFiles/client.dir/userservice.pb.cc.o"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/rpc/CMakeFiles/client.dir/userservice.pb.cc.o -MF CMakeFiles/client.dir/userservice.pb.cc.o.d -o CMakeFiles/client.dir/userservice.pb.cc.o -c /home/star/workspace/distribution_frame/app/rpc/userservice.pb.cc

app/rpc/CMakeFiles/client.dir/userservice.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/userservice.pb.cc.i"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/star/workspace/distribution_frame/app/rpc/userservice.pb.cc > CMakeFiles/client.dir/userservice.pb.cc.i

app/rpc/CMakeFiles/client.dir/userservice.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/userservice.pb.cc.s"
	cd /home/star/workspace/distribution_frame/build/app/rpc && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/star/workspace/distribution_frame/app/rpc/userservice.pb.cc -o CMakeFiles/client.dir/userservice.pb.cc.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client.cpp.o" \
"CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o" \
"CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o" \
"CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o" \
"CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o" \
"CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o" \
"CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o" \
"CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o" \
"CMakeFiles/client.dir/userservice.pb.cc.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

../bin/client: app/rpc/CMakeFiles/client.dir/client.cpp.o
../bin/client: app/rpc/CMakeFiles/client.dir/__/__/src/tools/lock.cpp.o
../bin/client: app/rpc/CMakeFiles/client.dir/__/__/src/tools/logger.cpp.o
../bin/client: app/rpc/CMakeFiles/client.dir/__/__/src/rpc/init.cpp.o
../bin/client: app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcheader.pb.cc.o
../bin/client: app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcprovider.cpp.o
../bin/client: app/rpc/CMakeFiles/client.dir/__/__/src/rpc/zookeeperutil.cpp.o
../bin/client: app/rpc/CMakeFiles/client.dir/__/__/src/rpc/rpcchannel.cpp.o
../bin/client: app/rpc/CMakeFiles/client.dir/userservice.pb.cc.o
../bin/client: app/rpc/CMakeFiles/client.dir/build.make
../bin/client: /usr/lib/x86_64-linux-gnu/libprotobuf.so
../bin/client: /usr/lib/x86_64-linux-gnu/libzookeeper_mt.a
../bin/client: ../lib/libmuduo_net.a
../bin/client: ../lib/libmuduo_base.a
../bin/client: ../lib/libyaml-cpp.a
../bin/client: app/rpc/CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/star/workspace/distribution_frame/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable ../../../bin/client"
	cd /home/star/workspace/distribution_frame/build/app/rpc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/rpc/CMakeFiles/client.dir/build: ../bin/client
.PHONY : app/rpc/CMakeFiles/client.dir/build

app/rpc/CMakeFiles/client.dir/clean:
	cd /home/star/workspace/distribution_frame/build/app/rpc && $(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : app/rpc/CMakeFiles/client.dir/clean

app/rpc/CMakeFiles/client.dir/depend:
	cd /home/star/workspace/distribution_frame/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/star/workspace/distribution_frame /home/star/workspace/distribution_frame/app/rpc /home/star/workspace/distribution_frame/build /home/star/workspace/distribution_frame/build/app/rpc /home/star/workspace/distribution_frame/build/app/rpc/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/rpc/CMakeFiles/client.dir/depend
