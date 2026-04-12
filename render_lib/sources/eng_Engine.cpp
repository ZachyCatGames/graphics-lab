#include <engine/eng_Engine.h>

#include <engine/raytracer/eng_ObjectFactory.h>
#include <engine/raytracer/eng_ThreadedRenderer.h>

namespace eng {

int Engine::Initialize(int argc, char** argv) {
    /* Initialize Graphics Args. */
    m_gArgs.process(argc, argv);

    /* Setup Object Factory. */
    m_pObjFactory = std::make_unique<rt::ObjectFactory>(this);

    /* Create the initial scene. */
    m_pActiveScene = std::make_shared<Scene>();

    /* Setup renderer. */
    m_pRenderer = std::make_unique<rt::ThreadedRenderer>(m_pActiveScene, m_gArgs.rpp, m_gArgs.randpix, m_gArgs.recursionDepth, m_gArgs.numCpus);

    return 0;
}

} // namespace eng
