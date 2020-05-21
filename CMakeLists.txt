# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.9)

project ("Enigma" LANGUAGES CXX CUDA)
set_property(GLOBAL PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	include(${TBB_ROOT}/cmake/TBBBuild.cmake)
	tbb_build(TBB_ROOT ${TBB_ROOT} CONFIG_DIR TBB_DIR MAKE_ARGS tbb_cpf=1)
	find_package(TBB REQUIRED tbb_preview)
endif()

# Include sub-projects.
add_subdirectory ("Enigma")
add_subdirectory ("Common")
add_subdirectory ("Test")
add_subdirectory ("gpu")
