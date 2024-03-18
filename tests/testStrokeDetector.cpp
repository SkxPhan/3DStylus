#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

#include "StrokeDetector.hpp"

// addTemplate

// getNumberOfTemplate

// recognize
TEST_CASE("Recognize rectangle") {
  StrokeDetector detector("tests/testdata");
  dollar::Stroke testStroke{{{0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}},
                            dollar::Orientation::Sensitive};
  std::string label = detector.recognize(testStroke);
  CHECK(label == "rectangle");
}

TEST_CASE("Recognize triangle") {
  StrokeDetector detector("tests/testdata");
  dollar::Stroke testStroke{{{0., 0.}, {1., 1.}, {2., 0.}, {0., 0.}},
                            dollar::Orientation::Sensitive};
  std::string label = detector.recognize(testStroke);
  CHECK(label == "triangle");
}