#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::shdr {

class Diffuse : public IShader, public ObjectBase<Diffuse> {
public:
    Diffuse(Handle<IShader> base_shader)
        : m_shader(base_shader) {}

    virtual Vector3DF GetColor(Scene* p_scene, int depth, const HitStruct& rec) override;
private:
    Handle<IShader> m_shader;
}; // class Diffuse

} // namespace eng
