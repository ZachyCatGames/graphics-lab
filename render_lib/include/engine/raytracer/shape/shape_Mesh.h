#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Vertex.h>
#include <engine/raytracer/eng_Bvh.h>
#include <engine/raytracer/eng_IShape.h>
#include <engine/raytracer/eng_ShapeBase.h>
#include <engine/raytracer/shape/shape_Triangle.h>

namespace eng::rt::shape {

class Mesh final : public ShapeBase, public ObjectBase<Mesh> {
public:
    Mesh(const std::vector<Vertex>& verts, Vector3DF positionOffset, Handle<IShader> shader);

    Mesh(const std::vector<Triangle>& tris, Vector3DF position, Handle<IShader> shader);

    virtual ~Mesh();

    virtual bool Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const override;

    virtual Vector3DF GetPosition() const override;
    virtual Bounds GetBounds() const override;
private:
    std::vector<Triangle> m_Triangles;
    Bvh m_Bvh;
    Vector3DF m_Position;
}; // class Mesh

} // namespace eng::rt::shape
