cmake_minimum_required(VERSION 3.12)
project(OpenCVExample)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Create the executable
add_executable(main src/main.cpp)

# Link the executable with OpenCV libraries
target_link_libraries(main ${OpenCV_LIBS})