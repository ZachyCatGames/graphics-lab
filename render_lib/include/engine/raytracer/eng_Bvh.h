#pragma once
#include <engine/eng_Bounds.h>
#include <engine/raytracer/eng_IShape.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Vector3D.h>
#include <algorithm>
#include <ranges>

namespace eng::rt {

class Bvh;

namespace detail {

class BvhNode : public IShape, public ObjectBase<BvhNode> {
public:
    constexpr BvhNode() :
        m_left_child(nullptr),
        m_right_child(nullptr),
        m_pParent(nullptr) {}

    //BvhNode(const BvhNode&) = delete;
    //BvhNode(BvhNode&&) = delete;

    template<std::ranges::random_access_range R>
    BvhNode(Bvh* pParent, R&& objects, int axis) : m_pParent(pParent) {
        this->Initialize(pParent, std::forward<R&&>(objects), axis);
    }

    virtual ~BvhNode() = default;

    template<std::ranges::random_access_range R>
    void Initialize(Bvh* pParent, R&& objects, int axis);

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;

    virtual Vector3DF GetPosition() const override { return m_bounds.GetCenter(); }
    virtual Bounds GetBounds() const override { return m_bounds; }

    /* This shouldn't ever be used. */
    virtual Handle<eng::IShader> GetShader() const override { return nullptr; }
private:
    friend class Bvh;
    constexpr bool Hit(const Ray& r) const { return m_bounds.Collides(r); }
private:
    IShape* m_left_child, * m_right_child;
    Bounds m_bounds;
    Bvh* m_pParent;
}; // class BvhNode

} // namespace detail

class Bvh : public IShape {
public:
    constexpr Bvh() :
        m_Allocator(),
        m_pNodes(nullptr),
        m_pLastNode(nullptr),
        m_NodeCount(0) {}

    Bvh(const Bvh&) = delete;
    Bvh(Bvh&&) = delete;

    template<std::ranges::random_access_range R>
    Bvh(R&& objects) : m_NodeCount(objects.size() * 2) {
        m_pNodes = m_Allocator.allocate(m_NodeCount);
        m_pLastNode = m_pNodes;
        std::construct_at(m_pLastNode++, this, std::forward<R&&>(objects), 0);
    }

    virtual ~Bvh();

    template<std::ranges::random_access_range R>
    void Initialize(R&& objects) {
        m_NodeCount = objects.size() * 2;
        m_pNodes = m_Allocator.allocate(m_NodeCount);
        m_pLastNode = m_pNodes;
        std::construct_at(m_pLastNode++, this, std::forward<R&&>(objects), 0);
    }

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override {
        return m_pNodes[0].Intersect(r, t_range, p_hit_info_out);
    }

    virtual Vector3DF GetPosition() const override {
        return m_pNodes[0].GetPosition();
    }

    virtual Bounds GetBounds() const override {
        return m_pNodes[0].GetBounds();
    }

    /* This shouldn't ever be used. */
    virtual Handle<eng::IShader> GetShader() const override { return nullptr; }
private:
    using NodeAllocator = std::allocator<detail::BvhNode>;

    friend class detail::BvhNode;
    NodeAllocator m_Allocator;
    detail::BvhNode* m_pNodes;
    detail::BvhNode* m_pLastNode;
    size_t m_NodeCount;
    //std::vector<detail::BvhNode> m_Nodes;
};

namespace detail {

template<std::ranges::random_access_range R>
void BvhNode::Initialize(Bvh* pParent, R&& objects, int axis) {
    m_pParent = pParent;
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
        auto chunks = std::ranges::views::chunk(objects, (n + 1) / 2);

        /* Setup left and right BvhNodes. */
        auto next_axis = (axis + 1) % 3;
        m_left_child  = std::construct_at(m_pParent->m_pLastNode++, m_pParent, chunks.front(), next_axis);
        m_right_child = std::construct_at(m_pParent->m_pLastNode++, m_pParent, chunks.back(), next_axis);

        /* Setup our bounds. */
        Bounds rightBounds = m_right_child->GetBounds();
        Bounds leftBounds  = m_left_child->GetBounds();
        m_bounds = rightBounds.Combine(leftBounds);
    }
}

} // namespace detail

} // namespace eng
