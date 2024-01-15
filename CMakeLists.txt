# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(OS2_F1_Simulator)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Add the executable
add_executable(Main main.c)

# Add the executable with test.c
add_executable(Test test.c)

# Add cJSON library
add_subdirectory(libs/cJSON/)

# Link against cJSON
target_link_libraries(Main PRIVATE cjson)

# Link against cJSON for TestExecutable if needed
target_link_libraries(Test PRIVATE cjson)

