# CMakeList.txt : CMake project for Enigma, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories("${PROJECT_SOURCE_DIR}/Common")
add_executable (Enigma "Enigma.cpp" )
add_executable (Arena "Arena.cpp" )
add_executable (Linear "Linear.cpp" )
add_executable (LetterCount "letter_count.cpp")

target_link_libraries(Enigma Common )
target_link_libraries(Arena Common )
target_link_libraries(Linear Common )

install(TARGETS Enigma DESTINATION $(PROJECT_SOURCE_DIR)/bin)