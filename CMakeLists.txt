cmake_minimum_required(VERSION 3.12)

project(3DStylus CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Specify the include directories
include_directories(include)

# Collect source files
file(GLOB SOURCES "src/*.cpp")
# Create an executable target
add_executable(main ${SOURCES})

# Link the executable with OpenCV libraries
target_link_libraries(main ${OpenCV_LIBS})