cmake_minimum_required (VERSION 3.9)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
include_directories("${PROJECT_SOURCE_DIR}/Common")

add_library (gpu STATIC "hillclimb_cuda.h" "hillclimb_cuda.cu")
set_target_properties( gpu PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

