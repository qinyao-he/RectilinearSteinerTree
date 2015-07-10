set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(zmst main.cpp ZMST.cpp ../SMST/PointsGenerator.cpp ../SMST/SMST.cpp)