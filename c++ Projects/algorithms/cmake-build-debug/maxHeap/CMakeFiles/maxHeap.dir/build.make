# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/xm210408/programs/algorithms

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/xm210408/programs/algorithms/cmake-build-debug

# Include any dependencies generated for this target.
include maxHeap/CMakeFiles/maxHeap.dir/depend.make

# Include the progress variables for this target.
include maxHeap/CMakeFiles/maxHeap.dir/progress.make

# Include the compile flags for this target's objects.
include maxHeap/CMakeFiles/maxHeap.dir/flags.make

maxHeap/CMakeFiles/maxHeap.dir/main.cpp.o: maxHeap/CMakeFiles/maxHeap.dir/flags.make
maxHeap/CMakeFiles/maxHeap.dir/main.cpp.o: ../maxHeap/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xm210408/programs/algorithms/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object maxHeap/CMakeFiles/maxHeap.dir/main.cpp.o"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/maxHeap && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/maxHeap.dir/main.cpp.o -c /Users/xm210408/programs/algorithms/maxHeap/main.cpp

maxHeap/CMakeFiles/maxHeap.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/maxHeap.dir/main.cpp.i"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/maxHeap && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xm210408/programs/algorithms/maxHeap/main.cpp > CMakeFiles/maxHeap.dir/main.cpp.i

maxHeap/CMakeFiles/maxHeap.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/maxHeap.dir/main.cpp.s"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/maxHeap && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xm210408/programs/algorithms/maxHeap/main.cpp -o CMakeFiles/maxHeap.dir/main.cpp.s

# Object files for target maxHeap
maxHeap_OBJECTS = \
"CMakeFiles/maxHeap.dir/main.cpp.o"

# External object files for target maxHeap
maxHeap_EXTERNAL_OBJECTS =

maxHeap/maxHeap: maxHeap/CMakeFiles/maxHeap.dir/main.cpp.o
maxHeap/maxHeap: maxHeap/CMakeFiles/maxHeap.dir/build.make
maxHeap/maxHeap: nameAge/libnameage.dylib
maxHeap/maxHeap: maxHeap/CMakeFiles/maxHeap.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xm210408/programs/algorithms/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable maxHeap"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/maxHeap && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/maxHeap.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
maxHeap/CMakeFiles/maxHeap.dir/build: maxHeap/maxHeap

.PHONY : maxHeap/CMakeFiles/maxHeap.dir/build

maxHeap/CMakeFiles/maxHeap.dir/clean:
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/maxHeap && $(CMAKE_COMMAND) -P CMakeFiles/maxHeap.dir/cmake_clean.cmake
.PHONY : maxHeap/CMakeFiles/maxHeap.dir/clean

maxHeap/CMakeFiles/maxHeap.dir/depend:
	cd /Users/xm210408/programs/algorithms/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xm210408/programs/algorithms /Users/xm210408/programs/algorithms/maxHeap /Users/xm210408/programs/algorithms/cmake-build-debug /Users/xm210408/programs/algorithms/cmake-build-debug/maxHeap /Users/xm210408/programs/algorithms/cmake-build-debug/maxHeap/CMakeFiles/maxHeap.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : maxHeap/CMakeFiles/maxHeap.dir/depend

