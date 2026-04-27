#pragma once
#include <engine/eng_Material.h>

namespace eng {

class IShader {
public:
    virtual ~IShader() = default;

    virtual const Material* GetMaterial() const = 0;
};

} // namespace eng
