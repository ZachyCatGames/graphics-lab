#include "NormalShader.h"

namespace eng {

Vector3DF NormalShader::GetColor(const HitStruct& rec) {
    return Vector3DF(0.5 * (rec.normal.normalize() + Vector3DF{1, 1, 1}));
}

} // namespace eng
