#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <string>

#include "dollar.hpp"
using namespace dollar;
using namespace std;

TEST_CASE("Recognize Stroke Test", "[recognize]") {
  SECTION("1 instance test") {
    vector<Stroke> strokes{
        Stroke({{0, 0}, {1, 1}, {1, 0}}, Orientation::Insensitive)};
    auto [it, score] = recognize(strokes[0], strokes.begin(), strokes.end());
    CHECK(it == strokes.begin());
    CHECK_THAT(score, Catch::Matchers::WithinAbs(0.63662f, 0.001f));
  }

  SECTION("2 instances test") {
    vector<Stroke> strokes{
        Stroke({{0., 0.}, {0., 1.}, {1., 1.}}, Orientation::Sensitive),
        Stroke({{0., 0.}, {1., 1.}, {1., 0.}}, Orientation::Sensitive),
    };
    Stroke testStroke{{{0., 0.}, {0., 0.9}, {0.02, 0.91}, {1., 1.}}, Orientation::Sensitive};
    auto [it, score] = recognize(testStroke, strokes.begin(), strokes.end());
    CHECK(it == strokes.begin());
    CHECK_THAT(score, Catch::Matchers::WithinAbs(0.63662f, 0.001f));
  }
}

TEST_CASE("Optimal Cosine Distance Test", "[optimalCosineDistance]") {
  SECTION("Simple test") {
    VectorizedStroke stroke1 = {{0.5, 0.5}, {1.5, 1.5}, {2.5, 2.5}, {3.5, 3.5}};
    VectorizedStroke stroke2 = {{0.7, 0.7}, {1.6, 1.6}, {2.6, 2.6}, {3.6, 3.6}};
    float expectedDistance = 1.5708f;

    float calculatedDistance = detail::optimalCosineDistance(stroke1, stroke2);

    REQUIRE_THAT(calculatedDistance,
                Catch::Matchers::WithinRel(expectedDistance));
  }

  SECTION("Symmetry Test") {
    VectorizedStroke stroke1 = {{0.5, 0.5}, {1.5, 1.5}, {2.5, 2.5}, {3.5, 3.5}};
    VectorizedStroke stroke2 = {{0.7, 0.7}, {1.6, 1.6}, {2.6, 2.6}, {3.6, 3.6}};

    float distance1 = detail::optimalCosineDistance(stroke1, stroke2);
    float distance2 = detail::optimalCosineDistance(stroke2, stroke1);

    REQUIRE_THAT(distance1, Catch::Matchers::WithinRel(distance2));
  }
}