# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Enigma")
set_property(GLOBAL PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include sub-projects.
add_subdirectory ("Enigma")
add_subdirectory ("Common")
add_subdirectory ("Test")

find_package(TBB REQUIRED)
