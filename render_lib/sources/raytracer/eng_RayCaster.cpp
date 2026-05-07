#include <engine/raytracer/eng_RayCaster.h>
#include <engine/raytracer/eng_HitStruct.h>
#include <engine/raytracer/eng_IShader.h>

namespace eng::rt {

Vector3DF RayCaster::CastRayIntoScene(const Ray& r, Interval<float> t_range, int depth) {
    /* Return black if we've reached the max recursion depth. */
    if (depth > m_maxDepth)
        printf("%d\n", depth);
    if (depth > m_maxDepth)
        return Vector3DF{0, 0, 0}; 

    /* Request the closest shape from our BVH. */
    HitStruct closest_rec;
    if(!m_Bvh.Intersect(r, t_range, &closest_rec)) {
        /* Return black if no shape was found. */
        return Vector3DF{0, 0, 0};
    }

    closest_rec.r = r;

    /* Run the shape's shader if available. */
    auto& shader = closest_rec.shader;
    if (shader) {
        auto c = shader->GetColor(this, depth, closest_rec);
        return c;
    }

    /* If no shader is available, fallback to returning white. */
    return Vector3DF{1.0, 1.0, 1.0};
}

bool RayCaster::IsObjectInPath(const Ray& r, Interval<float> t_range) {
    HitStruct rec;
    return m_Bvh.Intersect(r, t_range, &rec);
}

void RayCaster::PrepareBvhTree() {
    if(!m_scene->shapes.GetUpdateFlag())
        return;
    
    m_scene->shapes.ClearUpdateFlag();
    const auto& list = m_scene->shapes.GetList();

    /* Prepare pointer list. */
    std::vector<IShape*> pointers;
    pointers.reserve(list.size());
    for (auto handle : list) {
        pointers.push_back(static_cast<IShape*>(handle.Get()));
    }

    m_Bvh.Initialize(pointers);
}

} // namespace eng::rt
