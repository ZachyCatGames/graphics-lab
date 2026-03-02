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

    /* Initialize our camera. */
    m_camera = PerspectiveCamera(pos, dir, foc_len, m_img_width, m_img_height, vp_width);

    /* Initialize our framebuffer. */
    m_fb = fb::Framebuffer(m_img_width, m_img_height);

    /* Initialize our scene. */
    m_scene = Scene(m_args.recursionDepth, m_args.rpp, m_args.randpix);
}

void SceneRenderHelper::RenderScene() {
    std::atomic<int> cur_line = 0;
    const auto worker_func = [&]() {
        int line;
        while (line = cur_line++, line < m_img_height) {
            std::print("Line {}\n", line);
            for (int x = 0; x < m_img_width; x++) {
                auto color = m_scene.GetPixelColor(&m_camera, x, line);

                m_fb.SetPixelColor(x, line, color);
            }
        }
    };

    /* Render entire scene into the framebuffer. */
    {
        std::vector<std::jthread> threads(m_args.numCpus);
        for (auto& thread : threads)
            thread = std::jthread(worker_func);
    }
}

} // namespace eng::demo
