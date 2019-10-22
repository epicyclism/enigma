# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Enigma")
set_property(GLOBAL PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	include(${TBB_ROOT}/cmake/TBBBuild.cmake)
	tbb_build(TBB_ROOT ${TBB_ROOT} CONFIG_DIR TBB_DIR MAKE_ARGS tbb_cpf=1)
	find_package(TBB REQUIRED tbb_preview)
	message(STATUS "x TBB_ROOT = ${TBB_ROOT}")
	message(STATUS "x TBB_IMPORTED_TARGETS = ${TBB_IMPORTED_TARGETS}")
endif()

# Include sub-projects.
add_subdirectory ("Enigma")
add_subdirectory ("Common")
add_subdirectory ("Test")
