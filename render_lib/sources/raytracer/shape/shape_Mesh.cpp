#pragma once
#include <engine/raytracer/shape/shape_Mesh.h>

namespace eng::rt::shape {

Mesh::Mesh(const std::vector<float>& verts) : Mesh(MeshType::Mesh) {
    m_Triangles.reserve(verts.size() / 3 + 1);
    assert(verts.size() % 3 == 0);

    /* Construct the list of triangles. */
    for (size_t idx = 0; idx < verts.size(); idx += 3) {
        m_Triangles.emplace_back(verts[idx], verts[idx+1], verts[idx+2]);
    }

    /* Construct the BVH. */
    m_Bvh.Initialize(m_Triangles);
}

} // namespace eng::rt::shape
