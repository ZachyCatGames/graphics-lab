#pragma once
#include <engine/eng_HandleGraphicsArgs.h>
#include <engine/eng_IObjectFactory.h>
#include <engine/eng_IRenderer.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>
#include <memory>

namespace eng {

class Engine {
public:
    int Initialize(const GraphicsArgs& args);
    int Initialize(int argc, char** argv);

    IObjectFactory* GetObjectFactory() const { return m_pObjFactory.get(); }

    /* NOTE: This is for debugging!!! */
    IRenderer* GetRenderer() const { return m_pRenderer.get(); }

    const auto& GetActiveScene() const { return m_pActiveScene; }
    void UpdateActiveScene(const Handle<Scene>& scene);

    void RenderActiveScene(std::string_view cameraName, Handle<RenderBuffer> fb) {
        m_pRenderer->PreRender();

        m_pRenderer->Render(cameraName, fb);
    }
private:
    GraphicsArgs m_gArgs;

    std::shared_ptr<Scene> m_pActiveScene;

    std::unique_ptr<IObjectFactory> m_pObjFactory;
    std::unique_ptr<IRenderer> m_pRenderer;

    std::string m_renderMode;
}; // class Engine

} // namespace eng
