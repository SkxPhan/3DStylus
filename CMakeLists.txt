cmake_minimum_required(VERSION 3.20)

project(3DStylus)

option(ENABLE_DEV "Enable tests and other dev related features" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

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
  enable_testing()
  
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

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/_deps/catch2-src/extras)

  file(GLOB TEST_SOURCES "tests/*.cpp")
  add_executable(run_test ${TEST_SOURCES})
  target_compile_features(run_test PUBLIC cxx_std_17)
  target_link_libraries(run_test PRIVATE Catch2::Catch2WithMain dollar pugixml::pugixml)

  include(Catch)
  catch_discover_tests(run_test)
endif()
