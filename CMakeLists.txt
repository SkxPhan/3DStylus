cmake_minimum_required(VERSION 3.20)
project(3DStylus)

option(ENABLE_DEV "Enable tests and other dev related features" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library: dollar
add_library(dollar 
    include/dollar.hpp
    src/dollar.cpp
)
target_include_directories(dollar PUBLIC include)
target_compile_features(dollar PUBLIC cxx_std_23)

# Find OpenCV package
find_package(OpenCV REQUIRED)

include(cmake/CPM.cmake)
CPMAddPackage(
  NAME pugixml
  VERSION 1.14
  GITHUB_REPOSITORY zeux/pugixml
)

# Executable: main
add_executable(main 
    src/main.cpp
    src/ImageProcessing.cpp
    src/StrokeDetector.cpp
)
target_include_directories(main PUBLIC include)
target_link_libraries(main PRIVATE dollar ${OpenCV_LIBS} pugixml::pugixml)

# Tests
if (${ENABLE_DEV})
  enable_testing()
  
  # Library: Catch2
  CPMAddPackage("gh:catchorg/Catch2@3.4.0")

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/_deps/catch2-src/extras)

  # Tests executable
  add_executable(run_test 
    tests/testDollar.cpp
    tests/testStrokeDetector.cpp
    src/StrokeDetector.cpp
  )
  target_compile_features(run_test PUBLIC cxx_std_23)
  target_link_libraries(run_test PRIVATE Catch2::Catch2WithMain dollar pugixml::pugixml)
  include(Catch)
  catch_discover_tests(run_test)

  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests)
endif()
