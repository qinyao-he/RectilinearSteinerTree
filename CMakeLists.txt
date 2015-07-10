cmake_minimum_required(VERSION 3.2)
project(RectilinearSteinerTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

aux_source_directory(src SOURCE_FILES)
aux_source_directory(src/ImplementationA SOURCE_FILES)
aux_source_directory(src/ImplementationA/Headers SOURCE_FILES)
aux_source_directory(src/ImplementationA/LMST SOURCE_FILES)
aux_source_directory(src/ImplementationA/SMST SOURCE_FILES)
aux_source_directory(src/ImplementationA/ZMST SOURCE_FILES)
add_executable(RectilinearSteinerTree ${SOURCE_FILES})
