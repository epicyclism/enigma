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
include Test/CMakeFiles/MatchTest.dir/depend.make

# Include the progress variables for this target.
include Test/CMakeFiles/MatchTest.dir/progress.make

# Include the compile flags for this target's objects.
include Test/CMakeFiles/MatchTest.dir/flags.make

Test/CMakeFiles/MatchTest.dir/MatchTest.cpp.o: Test/CMakeFiles/MatchTest.dir/flags.make
Test/CMakeFiles/MatchTest.dir/MatchTest.cpp.o: Test/MatchTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Test/CMakeFiles/MatchTest.dir/MatchTest.cpp.o"
	cd /home/paul/enigma/Test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MatchTest.dir/MatchTest.cpp.o -c /home/paul/enigma/Test/MatchTest.cpp

Test/CMakeFiles/MatchTest.dir/MatchTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MatchTest.dir/MatchTest.cpp.i"
	cd /home/paul/enigma/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/enigma/Test/MatchTest.cpp > CMakeFiles/MatchTest.dir/MatchTest.cpp.i

Test/CMakeFiles/MatchTest.dir/MatchTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MatchTest.dir/MatchTest.cpp.s"
	cd /home/paul/enigma/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/enigma/Test/MatchTest.cpp -o CMakeFiles/MatchTest.dir/MatchTest.cpp.s

# Object files for target MatchTest
MatchTest_OBJECTS = \
"CMakeFiles/MatchTest.dir/MatchTest.cpp.o"

# External object files for target MatchTest
MatchTest_EXTERNAL_OBJECTS =

Test/MatchTest: Test/CMakeFiles/MatchTest.dir/MatchTest.cpp.o
Test/MatchTest: Test/CMakeFiles/MatchTest.dir/build.make
Test/MatchTest: Common/libCommon.a
Test/MatchTest: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbb_preview.so.2
Test/MatchTest: Test/CMakeFiles/MatchTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MatchTest"
	cd /home/paul/enigma/Test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MatchTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Test/CMakeFiles/MatchTest.dir/build: Test/MatchTest

.PHONY : Test/CMakeFiles/MatchTest.dir/build

Test/CMakeFiles/MatchTest.dir/clean:
	cd /home/paul/enigma/Test && $(CMAKE_COMMAND) -P CMakeFiles/MatchTest.dir/cmake_clean.cmake
.PHONY : Test/CMakeFiles/MatchTest.dir/clean

Test/CMakeFiles/MatchTest.dir/depend:
	cd /home/paul/enigma && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/enigma /home/paul/enigma/Test /home/paul/enigma /home/paul/enigma/Test /home/paul/enigma/Test/CMakeFiles/MatchTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Test/CMakeFiles/MatchTest.dir/depend
