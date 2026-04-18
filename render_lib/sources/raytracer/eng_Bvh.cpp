#include <engine/raytracer/eng_Bvh.h>

namespace eng::rt {

namespace detail {

bool BvhNode::Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const {
    if (!this->Hit(r))
        return false;

    /* Check left half. */
    HitStruct left_hit_info, right_hit_info;
    bool left_hit  = m_left_child->Intersect(r, t_range, &left_hit_info);
    bool right_hit = false;
    if(m_right_child)
        right_hit = m_right_child->Intersect(r, t_range, &right_hit_info);

    /* Determine which hitstruct we should return, if any. */
    if (left_hit && right_hit) {
        /* Prioritize whichever has the lower t (i.e., is closer). */
        if (left_hit_info.t > right_hit_info.t) {
            *p_hit_info_out = std::move(right_hit_info);
        } else {
            *p_hit_info_out = std::move(left_hit_info);
        }
    } else if (left_hit) {
        *p_hit_info_out = std::move(left_hit_info);
    } else if (right_hit) {
        *p_hit_info_out = std::move(right_hit_info);
    } else {
        return false;
    }

    return true;
}

} // namespace detail

Bvh::~Bvh() {
    /* Destroy all of the nodes. */
    detail::BvhNode* pNode = m_pNodes;
    while (pNode != m_pLastNode) {
        std::destroy_at(pNode++);
    }

    /* Free the array. */
    m_Allocator.deallocate(m_pNodes, m_NodeCount);
}

} // namespace eng::rt
