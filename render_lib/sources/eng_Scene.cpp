#include <engine/eng_Scene.h>

namespace eng {

Scene::Scene()
    : m_max_depth(4),
      m_samples_per_pixel(1),
      m_randomize_pixel_samples(false) {}

Scene::Scene(int max_depth, int samples_per_pixel, bool random_samples)
    : m_max_depth(max_depth),
      m_samples_per_pixel(samples_per_pixel),
      m_randomize_pixel_samples(random_samples) {}

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

Vector3DF Scene::GetPixelColor(ICamera* p_cam, int x, int y) {
    const auto subpix_stride = 1.0 / m_samples_per_pixel;
    const auto tmin          = p_cam->GetMinT();
    Rng<float> rng(0.0, subpix_stride);
    Vector3DF color_sum(0, 0, 0);
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

            /* Call GetRayColor starting at a depth of zero. */
            color_sum += this->GetRayColor(r, {tmin, std::numeric_limits<float>::infinity()}, 0);
        }
    }

    return color_sum / float(m_samples_per_pixel * m_samples_per_pixel);
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

void Scene::ReserveShapes(size_t count) {
    m_shapes.reserve(count);
}

} // namespace eng
