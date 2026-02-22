#include <engine/eng_Scene.h>

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

Vector3DF Scene::GetRayColor(const Ray& r) {
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

Vector3DF Scene::GetPixelColor(ICamera* p_cam, int x, int y) {
    const auto subpix_stride = 1.0 / m_samples_per_pixel;
    Rng<float> rng(0.0, subpix_stride);
    Vector3DF color_sum;
    for (int i = 0; i < m_samples_per_pixel; i++) {
        for (int j = 0; j < m_samples_per_pixel; j++) {
            /* Sample at a random point if random sampling is enabled, otherwise
               sample at the middle. */
            float x_offs, y_offs;
            if (m_randomize_pixel_samples) {
                x_offs = rng();
                y_offs = rng();
            } else {
                x_offs = subpix_stride / 2;
                y_offs = x_offs;
            }

            auto pos_x = x + j * subpix_stride + x_offs;
            auto pos_y = y + i * subpix_stride + y_offs;

            /* Generate a ray. */
            auto r = p_cam->GenerateRay(pos_x, pos_y);

            /* Call GetRayColor. */
            color_sum += this->GetRayColor(r);
        }
    }

    return color_sum / float(m_samples_per_pixel * m_samples_per_pixel);
}

} // namespace eng
