#include <engine/shader/shdr_NormalShader.h>

namespace eng::shdr {

Vector3DF Normal::GetColor(const HitStruct& rec) {
    return Vector3DF(0.5 * (rec.normal.normalize() + Vector3DF{1, 1, 1}));
}

} // namespace eng::shdr
