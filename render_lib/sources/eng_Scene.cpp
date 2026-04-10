#include <engine/eng_Scene.h>

namespace eng {

Scene::Scene() :
    m_objectsUpdated(false) {}

Scene::ObjectContext Scene::InsertShape(Handle<IShape> handle) {
    /* Mark the BVH as requiring an update. */
    m_objectsUpdated = true;

    m_shapes.push_back(handle);
    auto [iter, inserted] = m_attribs.insert_or_assign(handle, ShapeAttributes());
    return ObjectContext(iter);
}

bool Scene::ContainsShape(Handle<IShape> shape) {
    return m_attribs.contains(shape);
}

Scene::ObjectContext Scene::GetShapeContext(Handle<IShape> shape) {
    auto it = m_attribs.find(shape);
    if (it == m_attribs.end())
        return ObjectContext(it, false);
    return ObjectContext(it);
}

void Scene::RemoveShape(Handle<IShape> shape) {
    /* Mark the BVH as requiring an update. */
    m_objectsUpdated = true;

    /* Find where the shape is in the shape list. */
    auto it = std::ranges::find(m_shapes, shape);
    if (it == m_shapes.end())
        return;

    /* Remove it from the shapes list. */
    m_shapes.erase(it);

    /* Remove is from the attribs map. */
    m_attribs.erase(shape);
}

shdr::PointLight& Scene::EmplacePointLight(const Vector3DF& pos, const Vector3DF& intensity) {
    return m_lights.emplace_back(pos, intensity);
}

shdr::PointLight& Scene::InsertPointLight(const shdr::PointLight& light) {
    m_lights.push_back(light);
    return m_lights.back();
}

void Scene::ReserveShapes(size_t count) { m_shapes.reserve(count); }
void Scene::ReservePointLights(size_t count) { m_lights.reserve(count); }

} // namespace eng
