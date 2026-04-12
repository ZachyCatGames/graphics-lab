#include <engine/raytracer/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::rt::shdr {

class Diffuse : public IShader, public ObjectBase<Diffuse> {
public:
    Diffuse(Handle<IShader> base_shader)
        : m_shader(base_shader) {}

    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
private:
    Handle<IShader> m_shader;
}; // class Diffuse

} // namespace eng
