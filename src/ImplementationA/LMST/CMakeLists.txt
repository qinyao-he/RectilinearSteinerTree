cmake_minimum_required(VERSION 3.2)
project(RectilinearSteinerTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

aux_source_directory(src SOURCE_FILES)
aux_source_directory(test TEST_FILES)
add_executable(RectilinearSteinerTree ${SOURCE_FILES})
