#include <engine/raytracer/eng_RendererBase.h>

namespace eng::rt {

Vector3DF RendererBase::GetPixelColor(Handle<ICamera> p_cam, int x, int y) {
    const auto subpix_stride = 1.0 / m_raysPerPixel;
    const auto tmin          = p_cam->GetNearPlaneDistance();
    Rng<float> rng(0.0, subpix_stride);
    Vector3DF color_sum(0, 0, 0);
    for (int i = 0; i < m_raysPerPixel; i++) {
        for (int j = 0; j < m_raysPerPixel; j++) {
            /* Sample at a random point if random sampling is enabled, otherwise
               sample at the middle. */
            float x_offs, y_offs;
            if (m_randomSamples) {
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
            color_sum += m_caster.CastRayIntoScene(r, {tmin, std::numeric_limits<float>::infinity()}, 0);
        }
    }

    return color_sum / float(m_raysPerPixel * m_raysPerPixel);
}

} // namespace eng
