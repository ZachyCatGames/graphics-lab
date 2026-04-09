#include <engine/shape/shape_Sphere.h>
#include <print>

namespace eng::shape {

bool Sphere::Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const {
    Vector3DF oc = r.origin() - m_position;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0f * dot(oc, r.direction());
    auto c = dot(oc, oc) - m_radius * m_radius;
    auto disc = b*b - 4.0*a*c;

    /* Return if we didn't hit. */
    if (disc < 0)
        return false;

    auto disc_sqrt = std::sqrt(disc);
    float t        = (-b - disc_sqrt) / (2.0f * a);
    if (!t_range.Surrounds(t)) {
        t = (-b + disc_sqrt) / (2.0f * a);
        if (!t_range.Surrounds(t))
            return false;
    }

    p_hit_info_out->t        = t;
    p_hit_info_out->position = r.at(t);
    p_hit_info_out->normal   = (p_hit_info_out->position - m_position).normalize();

    return true;
}

Vector3DF Sphere::GetPosition() const { return m_position; }

Bounds Sphere::GetBounds() const {
    return Bounds {
        { m_position.x() - m_radius, m_position.x() + m_radius },
        { m_position.y() - m_radius, m_position.y() + m_radius },
        { m_position.z() - m_radius, m_position.z() + m_radius },
    };
}

} // namespace eng::shape
