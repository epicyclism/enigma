# CMakeList.txt : CMake project for Enigma, include source and define
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
install(TARGETS ModAlphaTest WiringTest RotorTest MachineTest DESTINATION $(PROJECT_SOURCE_DIR)/bin)