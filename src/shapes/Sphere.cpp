#include "Sphere.h"

#include <print>

namespace eng {

bool Sphere::Intersect(const ray& r, float tmin, float* p_tmax, HitStruct* p_hit_info_out) {
    Vector3DF oc = r.origin() - m_position;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - m_radius * m_radius;
    auto disc = b*b - 4*a*c;
    std::cout << oc << '\n';
    std::cout << r.direction() << '\n';
    std::print("{} {} {}\n", a, b, c);

    return disc >= 0;


    /* Return if we didn't hit. */
    if (disc < 0)
        return false;

    auto disc_sqrt = std::sqrt(disc);
    auto t1        = (b + disc_sqrt) / (2 * a);
    auto tmax      = *p_tmax;
    if (t1 >= tmin && t1 <= tmax) {
        *p_tmax = t1;
        return true;
    }

    auto t2 = (b - disc_sqrt) / (2 * a);
    if (t2 >= tmin && t2 <= tmax) {
        *p_tmax = t2;
        return true;
    }

    return false;
}

} // namespace eng
