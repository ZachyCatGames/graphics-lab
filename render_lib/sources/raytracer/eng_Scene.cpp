#include <engine/raytracer/eng_Scene.h>

namespace eng::rt {

Vector3DF Scene::GetRayColor(const Ray& r, Interval<float> t_range, int depth) {
    /* Return black if we've reached the max recursion depth. */
    if (depth > m_max_depth)
        return Vector3DF{0, 0, 0}; 

    /* Update the BVH if required. */
    if(m_objectsUpdated && m_bvhAutoUpdate)
        this->PrepareBvhTree();

    /* Request the closest shape from our BVH. */
    HitStruct closest_rec;
    if(!m_shapeBvh.Intersect(r, t_range, &closest_rec)) {
        /* Return black if no shape was found. */
        return Vector3DF{0, 0, 0};
    }

    /* Run the shape's shader if available. */
    auto& shader = closest_rec.shader;
    if (shader) {
        auto c = shader->GetColor(this, depth, closest_rec);
        return c;
    }

    /* If no shader is available, fallback to returning white. */
    return Vector3DF{1.0, 1.0, 1.0};
}

bool Scene::IsObjectInPath(const Ray& r, Interval<float> t_range) {
    HitStruct rec;
    return m_shapeBvh.Intersect(r, t_range, &rec);
}

void Scene::PrepareBvhTree() {
    if(!m_objectsUpdated)
        return;
    
    m_objectsUpdated = false;
    m_shapeBvh.Initialize(m_shapes);
}

} // namespace eng::rt
