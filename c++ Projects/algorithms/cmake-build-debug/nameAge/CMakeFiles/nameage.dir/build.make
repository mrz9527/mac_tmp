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
include nameAge/CMakeFiles/nameage.dir/depend.make

# Include the progress variables for this target.
include nameAge/CMakeFiles/nameage.dir/progress.make

# Include the compile flags for this target's objects.
include nameAge/CMakeFiles/nameage.dir/flags.make

nameAge/CMakeFiles/nameage.dir/NameAge.cpp.o: nameAge/CMakeFiles/nameage.dir/flags.make
nameAge/CMakeFiles/nameage.dir/NameAge.cpp.o: ../nameAge/NameAge.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/xm210408/programs/algorithms/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object nameAge/CMakeFiles/nameage.dir/NameAge.cpp.o"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/nameAge && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nameage.dir/NameAge.cpp.o -c /Users/xm210408/programs/algorithms/nameAge/NameAge.cpp

nameAge/CMakeFiles/nameage.dir/NameAge.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nameage.dir/NameAge.cpp.i"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/nameAge && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/xm210408/programs/algorithms/nameAge/NameAge.cpp > CMakeFiles/nameage.dir/NameAge.cpp.i

nameAge/CMakeFiles/nameage.dir/NameAge.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nameage.dir/NameAge.cpp.s"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/nameAge && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/xm210408/programs/algorithms/nameAge/NameAge.cpp -o CMakeFiles/nameage.dir/NameAge.cpp.s

# Object files for target nameage
nameage_OBJECTS = \
"CMakeFiles/nameage.dir/NameAge.cpp.o"

# External object files for target nameage
nameage_EXTERNAL_OBJECTS =

nameAge/libnameage.dylib: nameAge/CMakeFiles/nameage.dir/NameAge.cpp.o
nameAge/libnameage.dylib: nameAge/CMakeFiles/nameage.dir/build.make
nameAge/libnameage.dylib: nameAge/CMakeFiles/nameage.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/xm210408/programs/algorithms/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libnameage.dylib"
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/nameAge && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nameage.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
nameAge/CMakeFiles/nameage.dir/build: nameAge/libnameage.dylib

.PHONY : nameAge/CMakeFiles/nameage.dir/build

nameAge/CMakeFiles/nameage.dir/clean:
	cd /Users/xm210408/programs/algorithms/cmake-build-debug/nameAge && $(CMAKE_COMMAND) -P CMakeFiles/nameage.dir/cmake_clean.cmake
.PHONY : nameAge/CMakeFiles/nameage.dir/clean

nameAge/CMakeFiles/nameage.dir/depend:
	cd /Users/xm210408/programs/algorithms/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/xm210408/programs/algorithms /Users/xm210408/programs/algorithms/nameAge /Users/xm210408/programs/algorithms/cmake-build-debug /Users/xm210408/programs/algorithms/cmake-build-debug/nameAge /Users/xm210408/programs/algorithms/cmake-build-debug/nameAge/CMakeFiles/nameage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : nameAge/CMakeFiles/nameage.dir/depend

