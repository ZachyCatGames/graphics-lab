#include "Sphere.h"

#include <print>

namespace eng {

bool Sphere::Intersect(const ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const {
    Vector3DF oc = r.origin() - m_position;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - m_radius * m_radius;
    auto disc = b*b - 4*a*c;

    /* Return if we didn't hit. */
    if (disc < 0)
        return false;

    auto disc_sqrt = std::sqrt(disc);
    float t        = (-b - disc_sqrt) / (2 * a);
    if (!t_range.Surrounds(t)) {
        t = (b + disc_sqrt) / (2 * a);
        if (!t_range.Surrounds(t))
            return false;
    }

    p_hit_info_out->t        = t;
    p_hit_info_out->position = r.origin() + r.direction() * t;
    p_hit_info_out->normal   = (p_hit_info_out->position - m_position).normalize();

    return true;
}

} // namespace eng
