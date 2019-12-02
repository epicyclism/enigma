# CMakeList.txt : CMake project for Enigma, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories("${PROJECT_SOURCE_DIR}/Common")
add_executable (Enigma "Enigma.cpp" )
add_executable (Arena "Arena.cpp" )
add_executable (Linear "Linear.cpp" )
add_executable (LinearM "LinearM.cpp" )
add_executable (LinearE "LinearE.cpp" )
add_executable (LetterCount "letter_count.cpp")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	include(${TBB_ROOT}/cmake/TBBBuild.cmake)
	message(STATUS "TBB_ROOT = ${TBB_ROOT}")
	message(STATUS "TBB_IMPORTED_TARGETS = ${TBB_IMPORTED_TARGETS}")
#	target_link_libraries(Enigma Common tbb_preview)
#	target_link_libraries(Arena Common tbb_preview)
#	target_link_libraries(Linear Common tbb_preview)
#	target_link_libraries(LinearM Common tbb_preview)
	target_link_libraries(Enigma Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(Arena Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(Linear Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(LinearM Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(LinearE Common ${TBB_IMPORTED_TARGETS})
else()
	target_link_libraries(Enigma Common)
	target_link_libraries(Arena Common)
	target_link_libraries(Linear Common)
	target_link_libraries(LinearM Common)
	target_link_libraries(LinearE Common)
endif()

install(TARGETS Enigma Arena DESTINATION d:/projects/enigmachallenge)
