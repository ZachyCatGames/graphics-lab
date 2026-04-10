#pragma once
#include <engine/eng_IObjectFactory.h>
#include <memory>

namespace eng::detail {

void InitializeObjectFactory(std::unique_ptr<IObjectFactory>&& shapeFactory);

} // namespace eng::detail
