#include <engine/eng_Init.h>
#include <engine/detail/eng_CreateObject.h>
#include <print>

#include <engine/raytracer/eng_ObjectFactory.h>

namespace eng {

namespace {

int InitializeEngineRt() {
    detail::InitializeObjectFactory(std::make_unique<rt::ObjectFactory>());

    return 0;
}

} // namespace

int InitializeEngine(std::string_view renderString) {
    if(renderString.compare("raytracer"))
        return InitializeEngineRt();
    else {
        std::print("Invalid render mode!\n");
        return -1;
    }
}

} // namespace eng
