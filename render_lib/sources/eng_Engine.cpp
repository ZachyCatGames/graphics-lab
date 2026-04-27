#include <engine/eng_Engine.h>

#include <engine/raytracer/eng_ObjectFactory.h>
#include <engine/raytracer/eng_ThreadedRenderer.h>

#include <engine/gl/eng_ObjectFactory.h>
#include <engine/gl/eng_Renderer.h>

namespace eng {

int Engine::Initialize(int argc, char** argv) {
    /* Initialize Graphics Args. */
    m_gArgs.process(argc, argv);

    /* Create the initial scene. */
    m_pActiveScene = std::make_shared<Scene>();

    m_renderMode = m_gArgs.renderMode;

    if (m_renderMode == "raytracer") {
        /* Setup Object Factory. */
        m_pObjFactory = std::make_unique<rt::ObjectFactory>(this);

        /* Setup renderer. */
        m_pRenderer = std::make_unique<rt::ThreadedRenderer>(m_pActiveScene, m_gArgs.rpp, m_gArgs.randpix, m_gArgs.recursionDepth, m_gArgs.numCpus);
    } else if (m_renderMode == "opengl") {
        /* Setup Object Factory. */
        m_pObjFactory = std::make_unique<gl::ObjectFactory>();

        /* Setup renderer. */
        m_pRenderer = std::make_unique<gl::Renderer>(m_pActiveScene);
    }

    return 0;
}

} // namespace eng
