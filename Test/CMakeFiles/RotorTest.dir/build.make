# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/paul/enigma

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paul/enigma

# Include any dependencies generated for this target.
include Test/CMakeFiles/RotorTest.dir/depend.make

# Include the progress variables for this target.
include Test/CMakeFiles/RotorTest.dir/progress.make

# Include the compile flags for this target's objects.
include Test/CMakeFiles/RotorTest.dir/flags.make

Test/CMakeFiles/RotorTest.dir/RotorTest.cpp.o: Test/CMakeFiles/RotorTest.dir/flags.make
Test/CMakeFiles/RotorTest.dir/RotorTest.cpp.o: Test/RotorTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Test/CMakeFiles/RotorTest.dir/RotorTest.cpp.o"
	cd /home/paul/enigma/Test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RotorTest.dir/RotorTest.cpp.o -c /home/paul/enigma/Test/RotorTest.cpp

Test/CMakeFiles/RotorTest.dir/RotorTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RotorTest.dir/RotorTest.cpp.i"
	cd /home/paul/enigma/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/enigma/Test/RotorTest.cpp > CMakeFiles/RotorTest.dir/RotorTest.cpp.i

Test/CMakeFiles/RotorTest.dir/RotorTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RotorTest.dir/RotorTest.cpp.s"
	cd /home/paul/enigma/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/enigma/Test/RotorTest.cpp -o CMakeFiles/RotorTest.dir/RotorTest.cpp.s

# Object files for target RotorTest
RotorTest_OBJECTS = \
"CMakeFiles/RotorTest.dir/RotorTest.cpp.o"

# External object files for target RotorTest
RotorTest_EXTERNAL_OBJECTS =

Test/RotorTest: Test/CMakeFiles/RotorTest.dir/RotorTest.cpp.o
Test/RotorTest: Test/CMakeFiles/RotorTest.dir/build.make
Test/RotorTest: Test/CMakeFiles/RotorTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable RotorTest"
	cd /home/paul/enigma/Test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RotorTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Test/CMakeFiles/RotorTest.dir/build: Test/RotorTest

.PHONY : Test/CMakeFiles/RotorTest.dir/build

Test/CMakeFiles/RotorTest.dir/clean:
	cd /home/paul/enigma/Test && $(CMAKE_COMMAND) -P CMakeFiles/RotorTest.dir/cmake_clean.cmake
.PHONY : Test/CMakeFiles/RotorTest.dir/clean

Test/CMakeFiles/RotorTest.dir/depend:
	cd /home/paul/enigma && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/enigma /home/paul/enigma/Test /home/paul/enigma /home/paul/enigma/Test /home/paul/enigma/Test/CMakeFiles/RotorTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Test/CMakeFiles/RotorTest.dir/depend

