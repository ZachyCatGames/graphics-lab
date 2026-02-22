#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <engine/shape/shape_Sphere.h>

using namespace eng; // lazy

// TODO:
// In front of vmin
// Behind vmax
// Updates vmax
// Partially in [vmin, vmax]


TEST_CASE( "Basic Hit" )
{
    shape::Sphere sphere({0, 0, -15}, 5);
    constexpr Ray r({0, 0, 0}, {0.1, 0.1, 0.5});

    float vmax = std::numeric_limits<float>::max();
    REQUIRE(sphere.Intersect(r, Interval<float>(0, std::numeric_limits<float>::max()), nullptr) == true);
}

TEST_CASE( "Basic Miss" )
{
    shape::Sphere sphere({0, 0, -15}, 5);
    constexpr Ray r({0, 0, 0}, {0.5, 0.5, 0.5});

    float vmax = std::numeric_limits<float>::max();
    REQUIRE(sphere.Intersect(r, Interval<float>(0, std::numeric_limits<float>::max()), nullptr) == false);
}

