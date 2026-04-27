#include <engine/raytracer/shape/shape_Mesh.h>
#include <cassert>

namespace eng::rt::shape {

Mesh::Mesh(const std::vector<Vertex>& verts, Vector3DF positionOffset, Handle<IShader> shader) :
    m_Position(positionOffset),
    ShapeBase(shader.StaticCast<IShader>())
{
    assert(verts.size() % 3 == 0);

    /* Add each triangle with the offset. */
    for (int i = 0; i < verts.size(); i += 3) {
        m_Triangles.emplace_back(
            verts[i+0].position + positionOffset,
            verts[i+1].position + positionOffset,
            verts[i+2].position + positionOffset,
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
    ShapeBase(shader.StaticCast<IShader>())
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
