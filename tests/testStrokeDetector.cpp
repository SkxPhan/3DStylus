#include <catch2/catch_test_macros.hpp>
#include <string>
#include <string_view>

#include "StrokeDetector.hpp"

// addTemplate

// getNumberOfTemplate

// recognize

TEST_CASE("Recognize Simple Shapes") {
  const std::string_view strokeTemplatePath = "tests/testdata";
  StrokeDetector detector;
  detector.loadTemplateStrokes(strokeTemplatePath);

  SECTION("Recognize Rectangle") {
    dollar::Stroke rectangleStroke{{{0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}},
                                   dollar::Orientation::Sensitive};
    std::string rectangleLabel = detector.recognize(rectangleStroke);
    CHECK(rectangleLabel == "rectangle");
  }

  SECTION("Recognize Triangle") {
    dollar::Stroke triangleStroke{{{0., 0.}, {1., 1.}, {2., 0.}, {0., 0.}},
                                  dollar::Orientation::Sensitive};
    std::string triangleLabel = detector.recognize(triangleStroke);
    CHECK(triangleLabel == "triangle");
  }
}