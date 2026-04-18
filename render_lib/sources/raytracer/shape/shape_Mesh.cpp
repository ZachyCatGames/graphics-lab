#include <engine/raytracer/shape/shape_Mesh.h>
#include <cassert>

namespace eng::rt::shape {

Mesh::Mesh(const std::vector<float>& verts, Vector3DF positionOffset, Handle<IShader> shader) :
    m_Position(positionOffset),
    ShapeBase(shader)
{
    assert(verts.size() % 9 == 0);

    /* Add each triangle with the offset. */
    for (int i = 0; i < verts.size(); i += 9) {
        m_Triangles.emplace_back(
            Vector3DF(verts[i+0], verts[i+1], verts[i+2]) + positionOffset,
            Vector3DF(verts[i+3], verts[i+4], verts[i+5]) + positionOffset,
            Vector3DF(verts[i+6], verts[i+7], verts[i+8]) + positionOffset,
            nullptr
        );
    }

    /* Prepare triangle list. */
    std::vector<IShape*> pointers;
    pointers.reserve(m_Triangles.size());
    for (auto& tri : m_Triangles) {
        pointers.push_back(&tri);
    }

    /* Build our BVH. */
    m_Bvh.Initialize(pointers);
}

Mesh::Mesh(const std::vector<Triangle>& tris, Vector3DF position, Handle<IShader> shader) :
    m_Triangles(tris),
    m_Position(position),
    ShapeBase(shader)
{
    /* Prepare triangle list. */
    std::vector<IShape*> pointers;
    pointers.reserve(m_Triangles.size());
    for (auto& tri : m_Triangles) {
        pointers.push_back(&tri);
    }

    /* Build our BVH. */
    m_Bvh.Initialize(pointers);
}

Mesh::~Mesh() = default;

bool Mesh::Intersect(const Ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const {
    if (m_Bvh.Intersect(r, t_range, p_hit_info_out)) {
        p_hit_info_out->shader = this->GetShaderRT().Get();
        return true;
    }

    return false;
}

Vector3DF Mesh::GetPosition() const { return m_Position; }

Bounds Mesh::GetBounds() const { return m_Bvh.GetBounds(); }

} // namespace eng::rt::shape
