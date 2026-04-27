#pragma once
#include <engine/eng_Material.h>

namespace eng {

class IShader {
public:
    virtual ~IShader() = default;

    [[nodiscard]] virtual Material GetMaterial() const = 0;
};

} // namespace eng
