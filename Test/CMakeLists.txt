﻿# CMakeList.txt : CMake project for Enigma, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories("${PROJECT_SOURCE_DIR}/Common")
add_executable (ModAlphaTest "ModAlphaTest.cpp" )
add_executable (WiringTest "WiringTest.cpp" )
add_executable (RotorTest "RotorTest.cpp" )
add_executable (WheelSetTest "WheelSetTest.cpp" )
add_executable (MachineTest "MachineTest.cpp" )
add_executable (MatchTest "MatchTest.cpp" )
add_executable (score "Score.cpp"  )
add_executable (textgen "textgen.cpp"  )
add_executable (hillclimbtest "hillclimbtest.cpp"  )
add_executable (hillclimbtest2 "hillclimbtest2.cpp"  )


if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	include(${TBB_ROOT}/cmake/TBBBuild.cmake)
	target_link_libraries(MatchTest Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(hillclimbtest Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(hillclimbtest2 Common ${TBB_IMPORTED_TARGETS})
else()
	target_link_libraries(MatchTest Common)
	target_link_libraries(hillclimbtest Common)
	target_link_libraries(hillclimbtest2 Common)
endif()

target_link_libraries(MachineTest Common )
target_link_libraries(score Common)

#if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
#    target_compile_options( Enigma PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic -fexceptions)
#endif()
#if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
#    target_compile_options( Enigma PRIVATE -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn -fexceptions )
#endif()
#if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
#    target_compile_options( MachineTest PUBLIC /W4 )
#endif()

# TODO: Add tests and install targets if needed.
#install(TARGETS ModAlphaTest WiringTest RotorTest MachineTest MatchTest score DESTINATION $(PROJECT_SOURCE_DIR)/bin)
