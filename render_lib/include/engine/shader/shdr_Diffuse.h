#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

namespace eng::shdr {

class Diffuse : public IShader, public ObjectBase<Diffuse> {
public:
    virtual Vector3DF GetColor(Scene* p_scene, int depth, const HitStruct& rec) override;
private:
    
}; // class Diffuse

} // namespace eng
