#pragma once
#include <engine/eng_Bounds.h>
#include <engine/eng_IShape.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Vector3D.h>
#include <algorithm>
#include <ranges>

namespace eng {

namespace detail {

class BvhNode : public IShape, public ObjectBase<BvhNode> {
public:
    template<std::sortable R>
    BvhNode(R&& objects, int axis) {
        auto n = objects.size();
        if(n == 1) {
            m_left_child  = objects.front();
            m_right_child = nullptr;
            m_bounds = m_left_child->GetBounds();
        } else if(n == 2) {
            m_left_child  = objects.front();
            m_right_child = objects.back();
            m_bounds = m_left_child->GetBounds().Combine(m_right_child->GetBounds());
        } else {
            /* Sort our range by the target axis. */
            std::ranges::sort(objects, [axis](const auto& lhs, const auto& rhs) {
                return lhs->GetPosition()[axis] < rhs->GetPosition()[axis];
            });

            /* Chunk it. */
            auto chunks = std::ranges::views::chunk(objects, n / 2 + 1);

            /* Setup left and right BvhNodes. */
            auto next_axis = (axis + 1) % 3;
            m_left_child  = BvhNode::Create(chunks.front(), next_axis);
            m_right_child = BvhNode::Create(chunks.back(), next_axis);
        }
    }

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const;

    virtual Vector3DF GetPosition() const { return m_bounds.GetCenter(); }
    virtual Bounds GetBounds() const { return m_bounds; }
private:
    friend class Bvh;
    constexpr bool Hit(const Ray& r) const { return m_bounds.Collides(r); }
private:
    Handle<IShape> m_left_child, m_right_child;
    Bounds m_bounds;
}; // class BvhNode

} // namespace detail

class Bvh : public IShape {
public:
    template<std::sortable R>
    Bvh(R&& objects) : m_firstLevel(std::forward<R&&>(objects), 0) {
        /* ... */
    }

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const {
        return m_firstLevel.Intersect(r, t_range, p_hit_info_out);
    }

    virtual Vector3DF GetPosition() const { return m_firstLevel.GetPosition(); }
    virtual Bounds GetBounds() const { return m_firstLevel.GetBounds(); }
private:
    detail::BvhNode m_firstLevel;
};

} // namespace eng
