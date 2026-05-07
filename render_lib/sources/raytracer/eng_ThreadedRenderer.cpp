#include "engine/eng_ICamera.h"
#include <engine/raytracer/eng_ThreadedRenderer.h>
#include <engine/raytracer/eng_IDrawable.h>
#include <print>
#include <thread>

namespace eng::rt {

void ThreadedRenderer::PreRender() {
    /* Make sure the raycaster's BVH tree is ready. */
    m_caster.PrepareBvhTree();
}

void ThreadedRenderer::Render(std::string_view cameraName, Handle<RenderBuffer> fb) {
    assert(m_pScene);
    assert(m_caster.IsInitialized());
    assert(m_thread_count > 0);

    /* Find the target camera. */
    auto camera = m_pScene->cameras.Get(cameraName).StaticCast<ICamera>();

    /* Retrieve it's img dimensions. */
    auto [width, height] = camera->GetImageDimensions();

    /* Cast fb handle to an IDrawable handle. */
    Handle<IDrawable> drawable = fb.DynamicCast<IDrawable>();

    std::atomic<int> cur_line = 0;
    const auto worker_func = [&]() {
        int line;
        while (line = cur_line++, line < height) {
            std::print("Line {}\n", line);
            for (int x = 0; x < width; x++) {
                auto color = this->GetPixelColor(camera, x, line);

                drawable->SetPixelColor(x, line, color);
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
