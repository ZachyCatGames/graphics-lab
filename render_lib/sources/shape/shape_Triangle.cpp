#include <engine/shape/shape_Triangle.h>
#include <print>

namespace eng {

bool Triangle::Intersect(const ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const {
    const auto& dir  = r.direction();
    const auto& orig = r.origin();

    double a = m_a[0] - m_b[0];
  double b = m_a[1] - m_b[1];
  double c = m_a[2] - m_b[2];

  double d = m_a[0] - m_c[0];
  double e = m_a[1] - m_c[1];
  double f = m_a[2] - m_c[2];
  
  double g = r.direction()[0];
  double h = r.direction()[1];
  double i = r.direction()[2];

  double j = m_a[0] - r.origin()[0]; // correct
  double k = m_a[1] - r.origin()[1];
  double l = m_a[2] - r.origin()[2];

    const auto ei_minus_hf = e*i - h*f;
    const auto gf_minus_di = g*f - d*i;
    const auto dh_minus_eg = d*h - e*g;
    const auto ak_minus_jb = a*k - j*b;
    const auto jc_minus_al = j*c - a*l;
    const auto bl_minus_kc = b*l - k*c;

    const auto M = a * ei_minus_hf + b * gf_minus_di + c * (dh_minus_eg);

    const auto t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
    //std::cout << m_a << '\n' << m_b << '\n' << m_c << '\n';
    //std::cout << orig << '\n';
    //std::print("{} {} {}\n", j, k, l);
    //std::print("{}\n", t);
    //if (!t_range.Surrounds(t))
    if (!t_range.Surrounds(t))
        return false;

    const auto gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;

    if (!Interval<float>(0, 1).Surrounds(gamma))
        return false;

    const auto beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;

    if (beta < 0 || beta > (1 - gamma))
        return false;

    const auto u = m_b - m_a;
    const auto v = m_c - m_a;

    p_hit_info_out->normal   = cross(u, v).normalize();
    p_hit_info_out->position = r.direction() * float(t);
    p_hit_info_out->t        = t;

    return true;
}

} // namespace eng
