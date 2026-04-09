#pragma once
#include <engine/eng_Interval.h>
#include <engine/eng_Ray.h>
#include <engine/eng_Vector3D.h>

namespace eng {

namespace detail {

constexpr float CalculateTime(float pos, float origin, float direction) noexcept {
    return (pos - origin) / direction;
}

constexpr std::pair<float, float> CalculateTimeRange(Interval<float> range, float origin, float direction) {
    float tmin, tmax;
    if (direction >= 0) {
        tmin = CalculateTime(range.Min(), origin, direction);
        tmax = CalculateTime(range.Max(), origin, direction);
    } else {
        tmin = CalculateTime(range.Max(), origin, direction);
        tmax = CalculateTime(range.Min(), origin, direction);
    }
    return {tmin, tmax};
}

} // namespace detail

struct Bounds {
    Interval<float> x;
    Interval<float> y;
    Interval<float> z;

    constexpr Bounds Combine(const Bounds& other) const {
        auto merge_interval = [](const Interval<float>& first, const Interval<float>& second) {
            return Interval<float>(std::min(first.Min(), second.Min()), std::max(first.Max(), second.Max()));
        };

        return Bounds {
            merge_interval(this->x, other.x),
            merge_interval(this->y, other.y),
            merge_interval(this->z, other.z)
        };
    }

    constexpr bool Contains(const Vector3DF& point) {
        return x.Contains(point.x()) && y.Contains(point.y()) && z.Contains(point.z());
    }

    constexpr bool Collides(const Ray& ray) const {
        const auto& origin = ray.origin();
        const auto& dir    = ray.direction();
        
        /* Calculate time ranges on each axis. */
        auto [tx_min, tx_max] = detail::CalculateTimeRange(x, origin.x(), dir.x());
        auto [ty_min, ty_max] = detail::CalculateTimeRange(y, origin.y(), dir.y());
        auto [tz_min, tz_max] = detail::CalculateTimeRange(z, origin.z(), dir.z());

        if (tx_min > ty_max || ty_min > tx_max)
            return false;
        if (tz_min > ty_max || ty_min > tz_max)
            return false;
        if (tx_min > tz_max || tz_min > tx_max)
            return false;
        
        return true;
    }
}; // class Bounds

} // namespace eng
