# CMakeList.txt : CMake project for Enigma, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories("${PROJECT_SOURCE_DIR}/Common")

add_executable (Enigma "Enigma.cpp")
add_executable (Arena "Arena.cpp")
add_executable (Linear "Linear.cpp")
add_executable (LinearM "LinearM.cpp")
add_executable (LinearE "LinearE.cpp")
add_executable (LetterCount "letter_count.cpp")
add_executable (Arena2 "arena2.cpp")
add_executable (ioc "ioc.cpp")
add_executable (make_bgtg "make_bgtg.cpp")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	include(${TBB_ROOT}/cmake/TBBBuild.cmake)
	target_link_libraries(Enigma Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(Arena Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(Arena2 Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(Linear Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(LinearM Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(LinearE Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(ioc Common ${TBB_IMPORTED_TARGETS})
	target_link_libraries(make_bgtg Common ${TBB_IMPORTED_TARGETS})
else()
	target_link_libraries(Enigma Common)
	target_link_libraries(Arena Common)
	target_link_libraries(Arena2 Common)
	target_link_libraries(Linear Common)
	target_link_libraries(LinearM Common)
	target_link_libraries(LinearE Common)
	target_link_libraries(ioc Common)
	target_link_libraries(make_bgtg Common)
endif()

#install(TARGETS Enigma Arena DESTINATION d:/projects/enigmachallenge)
