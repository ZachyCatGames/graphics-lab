#include "Scene.h"

namespace eng {

Scene::ObjectContext Scene::InsertShape(Handle<IShape> handle) {
    m_shapes.push_back(handle);
    auto [iter, inserted] = m_attribs.insert_or_assign(handle, ShapeAttributes());
    return ObjectContext(iter);
}

bool Scene::Contains(Handle<IShape> shape) {
    return m_attribs.contains(shape);
}

Scene::ObjectContext Scene::GetContext(Handle<IShape> shape) {
    auto it = m_attribs.find(shape);
    if (it == m_attribs.end())
        return ObjectContext(it, false);
    return ObjectContext(it);
}

void Scene::Remove(Handle<IShape> shape) {
    /* Find where the shape is in the shape list. */
    auto it = std::ranges::find(m_shapes, shape);
    if (it == m_shapes.end())
        return;

    /* Remove it from the shapes list. */
    m_shapes.erase(it);

    /* Remove is from the attribs map. */
    m_attribs.erase(shape);
}

Vector3DF Scene::GetPixelColor(ICamera* p_cam, int x, int y) {
    /* Generate a ray. */
    auto r = p_cam->GenerateRay(x, y);

    float closest_found = std::numeric_limits<float>::infinity();
    Handle<IShape> closest_shape;
    HitStruct closest_rec;
    eng::HitStruct rec;
    for (auto& shape : m_shapes) {
        if (shape->Intersect(r, eng::Interval<float>(0.25, closest_found), &rec)) {
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
        if (shader) 
            return shader->GetColor(closest_rec) * Vector3DF(1.0, 0.60, 0.12);

        /* If no shader is available, fallback to returning white. */
        return Vector3DF{1.0, 1.0, 1.0};
    }

    /* Return black if no shape was found. */
    return Vector3DF{0.43, 1.0, 0.29};
}

} // namespace eng
