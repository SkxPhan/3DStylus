cmake_minimum_required(VERSION 3.20)

project(3DStylus)

option(ENABLE_DEV "Enable tests and other dev related features" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPM_DOWNLOAD_VERSION 0.38.7) 
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake")
  file(
      DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
      ${CPM_DOWNLOAD_LOCATION}
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})

add_library(dollar include/dollar.hpp src/dollar.cpp)
target_include_directories(dollar PUBLIC include)
target_compile_features(dollar PUBLIC cxx_std_23)

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
target_link_libraries(main ${OpenCV_LIBS} dollar)

if (${ENABLE_DEV})
  CPMAddPackage("gh:catchorg/Catch2@3.4.0")
  CPMAddPackage(
    NAME Format.cmake
    VERSION 1.7.3
    GITHUB_REPOSITORY TheLartians/Format.cmake
    OPTIONS 
        # set to yes skip cmake formatting
        "FORMAT_SKIP_CMAKE YES"
  )
  CPMAddPackage(
    NAME pugixml
    VERSION 1.14
    GITHUB_REPOSITORY zeux/pugixml
  )
  add_executable(test_dollar tests/test.cpp)
  target_compile_features(test_dollar PUBLIC cxx_std_17)
  target_link_libraries(test_dollar Catch2::Catch2WithMain dollar pugixml::pugixml)

  include(CTest)
  add_test(NAME test_dollar COMMAND ${CMAKE_BINARY_DIR}/test_dollar WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()