#pragma once
#include <engine/eng_Interval.h>
#include <engine/eng_Ray.h>
#include <engine/eng_Vector3D.h>

namespace eng {

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
        const auto& dir    = ray.inverse_direction();
        
        constexpr auto calc_interval = [](Interval<float> range, float origin, float direction) constexpr -> std::pair<float, float> {
            float t1 = (range.Min() - origin) * direction;
            float t2 = (range.Max() - origin) * direction;
        
            float tmin = std::min(t1, t2);
            float tmax = std::max(t1, t2);
        
            return {tmin, tmax};
        };

        /* Calculate time ranges on each axis. */
        auto [tx_min, tx_max] = calc_interval(x, origin.x(), dir.x());
        auto [ty_min, ty_max] = calc_interval(y, origin.y(), dir.y());
        auto [tz_min, tz_max] = calc_interval(z, origin.z(), dir.z());

        // TODO: CUDA; adopt a branch-less approach
        if (tx_min > ty_max || ty_min > tx_max)
            return false;
        if (tz_min > ty_max || ty_min > tz_max)
            return false;
        if (tx_min > tz_max || tz_min > tx_max)
            return false;
        
        return true;
    }

    constexpr Vector3DF GetCenter() const {
        return Vector3DF{
            (x.Min() + x.Max()) / 2,
            (y.Min() + y.Max()) / 2,
            (z.Min() + z.Max()) / 2,
        };
    }
}; // class Bounds

} // namespace eng
