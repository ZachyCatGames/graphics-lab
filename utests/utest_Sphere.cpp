#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "shapes/Sphere.h"

using namespace eng; // lazy

// TODO:
// In front of vmin
// Behind vmax
// Updates vmax
// Partially in [vmin, vmax]


TEST_CASE( "Basic Hit" )
{
    Sphere sphere({0, 0, -15}, 5);
    constexpr ray r({0, 0, 0}, {0.1, 0.1, 0.5});

    float vmax = std::numeric_limits<float>::max();
    REQUIRE(sphere.Intersect(r, 0, &vmax, nullptr) == true);
}

TEST_CASE( "Basic Miss" )
{
    Sphere sphere({0, 0, -15}, 5);
    constexpr ray r({0, 0, 0}, {0.5, 0.5, 0.5});

    float vmax = std::numeric_limits<float>::max();
    REQUIRE(sphere.Intersect(r, 0, &vmax, nullptr) == false);
}

