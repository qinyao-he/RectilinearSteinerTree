set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(test main.cpp ../SMST/SMST.cpp ../SMST/PointsGenerator.cpp ../LMST/LMST.cpp ../ZMST/ZMST.cpp)
