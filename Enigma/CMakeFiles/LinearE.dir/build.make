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
include Enigma/CMakeFiles/LinearE.dir/depend.make

# Include the progress variables for this target.
include Enigma/CMakeFiles/LinearE.dir/progress.make

# Include the compile flags for this target's objects.
include Enigma/CMakeFiles/LinearE.dir/flags.make

Enigma/CMakeFiles/LinearE.dir/LinearE.cpp.o: Enigma/CMakeFiles/LinearE.dir/flags.make
Enigma/CMakeFiles/LinearE.dir/LinearE.cpp.o: Enigma/LinearE.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Enigma/CMakeFiles/LinearE.dir/LinearE.cpp.o"
	cd /home/paul/enigma/Enigma && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LinearE.dir/LinearE.cpp.o -c /home/paul/enigma/Enigma/LinearE.cpp

Enigma/CMakeFiles/LinearE.dir/LinearE.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LinearE.dir/LinearE.cpp.i"
	cd /home/paul/enigma/Enigma && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/enigma/Enigma/LinearE.cpp > CMakeFiles/LinearE.dir/LinearE.cpp.i

Enigma/CMakeFiles/LinearE.dir/LinearE.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LinearE.dir/LinearE.cpp.s"
	cd /home/paul/enigma/Enigma && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/enigma/Enigma/LinearE.cpp -o CMakeFiles/LinearE.dir/LinearE.cpp.s

# Object files for target LinearE
LinearE_OBJECTS = \
"CMakeFiles/LinearE.dir/LinearE.cpp.o"

# External object files for target LinearE
LinearE_EXTERNAL_OBJECTS =

Enigma/LinearE: Enigma/CMakeFiles/LinearE.dir/LinearE.cpp.o
Enigma/LinearE: Enigma/CMakeFiles/LinearE.dir/build.make
Enigma/LinearE: Common/libCommon.a
Enigma/LinearE: tbb_cmake_build/tbb_cmake_build_subdir_release/libtbb_preview.so.2
Enigma/LinearE: Enigma/CMakeFiles/LinearE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable LinearE"
	cd /home/paul/enigma/Enigma && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LinearE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Enigma/CMakeFiles/LinearE.dir/build: Enigma/LinearE

.PHONY : Enigma/CMakeFiles/LinearE.dir/build

Enigma/CMakeFiles/LinearE.dir/clean:
	cd /home/paul/enigma/Enigma && $(CMAKE_COMMAND) -P CMakeFiles/LinearE.dir/cmake_clean.cmake
.PHONY : Enigma/CMakeFiles/LinearE.dir/clean

Enigma/CMakeFiles/LinearE.dir/depend:
	cd /home/paul/enigma && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/enigma /home/paul/enigma/Enigma /home/paul/enigma /home/paul/enigma/Enigma /home/paul/enigma/Enigma/CMakeFiles/LinearE.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Enigma/CMakeFiles/LinearE.dir/depend
