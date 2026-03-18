#include <engine/demo/demo_SceneRenderHelper.h>
#include <engine/demo/demo_Arguments.h>
#include <thread>
#include <vector>

namespace eng::demo {

SceneRenderHelper::SceneRenderHelper(int argc, char** argv) {
    /* Parse arguments. */
    auto [argc_real, argv_real] = demo::GetArguments(argc, argv);
    m_args.process(argc_real, argv_real);

    m_img_width  = m_args.width;
    m_img_height = m_args.height;

    static constexpr float vp_width  = 0.5;

    static constexpr auto pos = Vector3DF(0, 5, 0);
    static const auto dir = Vector3DF(0, 0, -1.0).normalize();
    static constexpr float foc_len = 0.2;

    /* Initialize our framebuffer. */
    m_fb = fb::Framebuffer(m_img_width, m_img_height);

    /* Initialize our scene. */
    m_scene = Scene(m_args.recursionDepth);

    /* Initialize our renderer. */
    m_renderer.Initialize(&m_scene, m_args.rpp, m_args.randpix, m_args.numCpus);

    /* Add a camera to our renderer. */
    m_renderer.EmplaceCamera<PerspectiveCamera>(pos, dir, foc_len, m_img_width, m_img_height, vp_width);
}

void SceneRenderHelper::RenderScene() {
    /* Render the scene using camera 0. */
    m_renderer.Render(0, &m_fb);
}

} // namespace eng::demo
