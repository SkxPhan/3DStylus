#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <string>

#include "dollar.hpp"
using namespace dollar;
using namespace std;

TEST_CASE("Dummy test", "[dummy]") {
  VectorizedStroke stroke1 = {{0.5, 0.5}, {1.5, 1.5}, {2.5, 2.5}, {3.5, 3.5}};
  VectorizedStroke stroke2 = {{0.7, 0.7}, {1.6, 1.6}, {2.6, 2.6}, {3.6, 3.6}};

  float distance1 = detail::optimalCosineDistance(stroke1, stroke2);
  float distance2 = detail::optimalCosineDistance(stroke2, stroke1);

  REQUIRE_THAT(distance1, Catch::Matchers::WithinRel(distance2));
}