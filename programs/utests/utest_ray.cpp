#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <engine/eng_Ray.h>

using namespace eng; // lazy

TEST_CASE( "Origin Test" )
{
    static constexpr Vector3DF origin    {4.2, 6.9, 9001};
    static constexpr Vector3DF direction {3.14, 15.0, 11.1};

    static constexpr Ray r {origin, direction};

    // these really should be exactly equal but _shrug_
    static constexpr auto orig_eq = equal_exact(origin, r.origin());
    static_assert(orig_eq);
    
    REQUIRE(orig_eq); // yay
}

TEST_CASE( "Direction Test" )
{
    static constexpr Vector3DF origin    {4.2, 6.9, 9001};
    static constexpr Vector3DF direction {3.14, 15.0, 11.1};

    static constexpr Ray r {origin, direction};

    // these really should be exactly equal but _shrug_
    static constexpr auto dir_eq = equal_exact(direction, r.direction());
    static_assert(dir_eq);
    
    REQUIRE(dir_eq); // yay
}

template<typename R, typename V>
concept OriginMutable = requires(R&& r, V&& v) {
    r.origin() = v;
};

template<typename R, typename V>
concept DirectionMutable = requires(R&& r, V&& v) {
    r.direction() = v;
};

template<typename R, typename V>
constexpr bool OM() {
    return requires(R&& r, V&& v) {
        r.origin() = v;
    };
}

TEST_CASE( "Immutable Test" )
{
    static constexpr Vector3DF origin    {4.2, 6.9, 9001};
    static constexpr Vector3DF direction {3.14, 15.0, 11.1};

    static constexpr Ray r {origin, direction};

    static constexpr bool r3 = !OM<Ray, Vector3DF>();
    static_assert(r3);

    /* Test that references to origin and direction are immutable. */
    static_assert(!OriginMutable<Ray, Vector3DF>,    "Origin Reference Mutable");
    static_assert(!DirectionMutable<Ray, Vector3DF>, "Direction Reference Mutable");

    /* Test that the at function doesn't modify anything. */
    static_assert([]() {
        Vector3DF v;
        Ray r1 = r;
        for (float i = 0.0; i < 4000; i += 0.32352958) {
            v += r.at(i);
        }
        // don't care about the x equals case, just trying to avoid optimizer bs
        return equal_exact(r1.origin(), r.origin()) && equal_exact(r1.direction(), r.direction()) && v.x() != 0;
    }());

    /* If we build, we pass... */
    REQUIRE(true); // yay
}

TEST_CASE( "at(0)" )
{
    static constexpr Vector3DF origin    {4.2, 6.9, 9001};
    static constexpr Vector3DF direction {3.14, 15.0, 11.1};

    static constexpr Ray r {origin, direction};

    static constexpr auto v = r.at(0);

    static_assert(equal_within(origin, v, 1.0e-4));

    /* If we build, we pass... */
    REQUIRE(true); // yay
}

TEST_CASE( "at(31.415)" )
{
    static constexpr Vector3DF origin    {4.2, 6.9, 9001};
    static constexpr Vector3DF direction {3.14, 15.0, 11.1};

    static constexpr Ray r {origin, direction};

    static constexpr auto v = r.at(31.415);

    static constexpr Vector3DF expected { 102.8431, 478.125, 9349.7065 };

    static_assert(equal_within(expected, v, 1.0e-4));

    /* If we build, we pass... */
    REQUIRE(true); // yay
}

TEST_CASE( "at(-14.36)" )
{
    static constexpr Vector3DF origin    {4.2, 6.9, 9001};
    static constexpr Vector3DF direction {3.14, 15.0, 11.1};

    static constexpr Ray r {origin, direction};

    static constexpr auto v = r.at(-14.36);

    static constexpr Vector3DF expected { -40.8904, -208.5, 8841.604 };

    static_assert(equal_within(expected, v, 1.0e-4));

    /* If we build, we pass... */
    REQUIRE(true); // yay
}
