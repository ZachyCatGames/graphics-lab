#pragma once
#include <engine/eng_IShape.h>
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectManager.h>

namespace eng {

class IObjectFactory {
public:
    virtual ~IObjectFactory() = default;
    
    virtual Handle<IShape> CreateSphere(const Vector3DF& pos, float radius, Handle<IShader> shader) = 0;
    virtual Handle<IShape> CreateTriangle(const Vector3DF& a, const Vector3DF& b, const Vector3DF& c, Handle<IShader> shader) = 0;
}; // class IObjectFactory

} // namespace eng
