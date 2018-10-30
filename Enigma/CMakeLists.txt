# CMakeList.txt : CMake project for Enigma, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories("${PROJECT_SOURCE_DIR}/Common")
add_executable (Enigma "Enigma.cpp" "Enigma.h")

# TODO: Add tests and install targets if needed.
