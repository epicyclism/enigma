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
include Enigma/CMakeFiles/LetterCount.dir/depend.make

# Include the progress variables for this target.
include Enigma/CMakeFiles/LetterCount.dir/progress.make

# Include the compile flags for this target's objects.
include Enigma/CMakeFiles/LetterCount.dir/flags.make

Enigma/CMakeFiles/LetterCount.dir/letter_count.cpp.o: Enigma/CMakeFiles/LetterCount.dir/flags.make
Enigma/CMakeFiles/LetterCount.dir/letter_count.cpp.o: Enigma/letter_count.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Enigma/CMakeFiles/LetterCount.dir/letter_count.cpp.o"
	cd /home/paul/enigma/Enigma && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LetterCount.dir/letter_count.cpp.o -c /home/paul/enigma/Enigma/letter_count.cpp

Enigma/CMakeFiles/LetterCount.dir/letter_count.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LetterCount.dir/letter_count.cpp.i"
	cd /home/paul/enigma/Enigma && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paul/enigma/Enigma/letter_count.cpp > CMakeFiles/LetterCount.dir/letter_count.cpp.i

Enigma/CMakeFiles/LetterCount.dir/letter_count.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LetterCount.dir/letter_count.cpp.s"
	cd /home/paul/enigma/Enigma && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paul/enigma/Enigma/letter_count.cpp -o CMakeFiles/LetterCount.dir/letter_count.cpp.s

# Object files for target LetterCount
LetterCount_OBJECTS = \
"CMakeFiles/LetterCount.dir/letter_count.cpp.o"

# External object files for target LetterCount
LetterCount_EXTERNAL_OBJECTS =

Enigma/LetterCount: Enigma/CMakeFiles/LetterCount.dir/letter_count.cpp.o
Enigma/LetterCount: Enigma/CMakeFiles/LetterCount.dir/build.make
Enigma/LetterCount: Enigma/CMakeFiles/LetterCount.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paul/enigma/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable LetterCount"
	cd /home/paul/enigma/Enigma && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LetterCount.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Enigma/CMakeFiles/LetterCount.dir/build: Enigma/LetterCount

.PHONY : Enigma/CMakeFiles/LetterCount.dir/build

Enigma/CMakeFiles/LetterCount.dir/clean:
	cd /home/paul/enigma/Enigma && $(CMAKE_COMMAND) -P CMakeFiles/LetterCount.dir/cmake_clean.cmake
.PHONY : Enigma/CMakeFiles/LetterCount.dir/clean

Enigma/CMakeFiles/LetterCount.dir/depend:
	cd /home/paul/enigma && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paul/enigma /home/paul/enigma/Enigma /home/paul/enigma /home/paul/enigma/Enigma /home/paul/enigma/Enigma/CMakeFiles/LetterCount.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Enigma/CMakeFiles/LetterCount.dir/depend

