#include <engine/raytracer/eng_RendererBase.h>

namespace eng::rt {

void RendererBase::Initialize(Scene* p_scene, size_t default_img_width, size_t default_img_height, size_t samples_per_pixel, bool random_samples) {
    m_p_scene = p_scene;

    m_default_img_width  = default_img_width;
    m_default_img_height = default_img_height;

    m_samples_per_pixel = samples_per_pixel;
    m_randomize_samples = random_samples;

    m_current_camera_id = 0;
}

void RendererBase::Finalize() {
    /* Clear scene pointer so we're treated as uninitialized. */
    m_p_scene = nullptr;

    /* Clear camera list. */
    m_cameras.clear();
}

RendererBase::~RendererBase() = default;

int RendererBase::InsertCamera(Handle<ICamera> camera) {
    m_cameras[m_current_camera_id] = camera;
    return m_current_camera_id++;
}

bool RendererBase::IsCameraIdValid(int id) {
    return m_cameras.contains(id);
}

void RendererBase::RemoveCamera(int id) {
    /* Remove the camera with ID. */
    m_cameras.erase(id);
}

Vector3DF RendererBase::GetPixelColor(Handle<ICamera> p_cam, int x, int y) {
    const auto subpix_stride = 1.0 / m_samples_per_pixel;
    const auto tmin          = p_cam->GetMinT();
    Rng<float> rng(0.0, subpix_stride);
    Vector3DF color_sum(0, 0, 0);
    for (int i = 0; i < m_samples_per_pixel; i++) {
        for (int j = 0; j < m_samples_per_pixel; j++) {
            /* Sample at a random point if random sampling is enabled, otherwise
               sample at the middle. */
            float x_offs, y_offs;
            if (m_randomize_samples) {
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
            color_sum += m_p_scene->GetRayColor(r, {tmin, std::numeric_limits<float>::infinity()}, 0);
        }
    }

    return color_sum / float(m_samples_per_pixel * m_samples_per_pixel);
}

} // namespace eng
