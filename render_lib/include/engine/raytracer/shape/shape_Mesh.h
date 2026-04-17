#pragma once
#include <engine/raytracer/eng_Bvh.h>
#include <engine/raytracer/eng_Shape.h>
#include "shape_Triangle.h"

namespace eng::rt::shape {

class Mesh : public Shape {
public:
    Mesh(const std::vector<float>& verts);

    ~Mesh() = default;

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;

    Bounds GetBounds() const;
private:
    std::vector<Triangle> m_Triangles;
    Bvh m_Bvh;
}; // class Mesh

} // namespace eng::rt::shape
