#include <engine/eng_Scene.h>

namespace eng {

Scene::Scene()
    : m_max_depth(4) {}

Scene::Scene(int max_depth)
    : m_max_depth(max_depth) {}

Scene::ObjectContext Scene::InsertShape(Handle<IShape> handle) {
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

Vector3DF Scene::GetRayColor(const Ray& r, Interval<float> t_range, int depth) {
    /* Return black if we've reached the max recursion depth. */
    if (depth > m_max_depth)
        return Vector3DF{0, 0, 0}; 
   
    float closest_found = t_range.Max();
    Handle<IShape> closest_shape;
    HitStruct closest_rec;
    eng::HitStruct rec;
    for (auto& shape : m_shapes) {
        if (shape->Intersect(r, eng::Interval<float>(t_range.Min(), closest_found), &rec)) {
            closest_found = rec.t;
            closest_shape = shape;
            closest_rec   = rec;

            /* Populate ray field of hitstruct. */
            closest_rec.r = r;
        }
    }

    /* Check if a shape was found. */
    if (closest_shape.IsValid()) {
        /* Run the shape's shader if available. */
        auto shader = m_attribs[closest_shape].shader;
        if (shader) {
            auto c = shader->GetColor(this, depth, closest_rec);
            return c;
        }

        /* If no shader is available, fallback to returning white. */
        return Vector3DF{1.0, 1.0, 1.0};
    }

    /* Return black if no shape was found. */
    return Vector3DF{0, 0, 0};
}

bool Scene::IsObjectInPath(const Ray& r, Interval<float> t_range) {
    HitStruct rec;
    for (auto& shape : m_shapes) {
        if (shape->Intersect(r, t_range, &rec)) {
            return true;
        }
    }

    return false;
}

void Scene::ReserveShapes(size_t count) { m_shapes.reserve(count); }
void Scene::ReservePointLights(size_t count) { m_lights.reserve(count); }

} // namespace eng
