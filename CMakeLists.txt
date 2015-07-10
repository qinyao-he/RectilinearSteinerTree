cmake_minimum_required(VERSION 3.2)
project(RectilinearSteinerTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)
set(OpenCV_INCLUDE_DIRS "${OpenCV_CONFIG_PATH}/include" "${OpenCV_CONFIG_PATH}/include/opencv")
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(src SOURCE_FILES)
aux_source_directory(src/ImplementationA SOURCE_FILES)
aux_source_directory(src/ImplementationA/Headers SOURCE_FILES)
aux_source_directory(src/ImplementationA/LMST SOURCE_FILES)
aux_source_directory(src/ImplementationA/SMST SOURCE_FILES)
aux_source_directory(src/ImplementationA/ZMST SOURCE_FILES)

add_executable(RectilinearSteinerTree ${SOURCE_FILES})
target_link_libraries(RectilinearSteinerTree ${OpenCV_LIBS})