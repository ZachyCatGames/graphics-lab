#include <engine/raytracer/eng_ShaderBase.h>
#include <engine/eng_Handle.h>
#include <engine/eng_Material.h>

namespace eng::rt::shdr {

class Diffuse : public ShaderBase {
public:
    Diffuse(const Material& material)
        : ShaderBase(material) {}

    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
}; // class Diffuse

} // namespace eng
