#include <engine/raytracer/eng_ThreadedRenderer.h>
#include <thread>

namespace eng::rt {

void ThreadedRenderer::Render(int camera_id, fb::Framebuffer* p_fb) {
    /* Find the target camera. */
    auto camera = m_cameras[camera_id];

    /* Retrieve it's img dimensions. */
    auto [width, height] = camera->GetImageDimensions();
    
    /* Update the scene's bvh. */
    m_p_scene->PrepareBvhTree();

    std::atomic<int> cur_line = 0;
    const auto worker_func = [&]() {
        int line;
        while (line = cur_line++, line < height) {
            std::print("Line {}\n", line);
            for (int x = 0; x < width; x++) {
                auto color = this->GetPixelColor(camera, x, line);

                p_fb->SetPixelColor(x, line, color);
            }
        }
    };

    /* Render entire scene into the framebuffer. */
    {
        std::vector<std::jthread> threads(m_thread_count);
        for (auto& thread : threads)
            thread = std::jthread(worker_func);
    }
}

} // namespace eng
