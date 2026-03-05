#pragma once

#include <engine/eng_ISceneLoader.h>
#include <engine/eng_Scene.h>

namespace eng {

class SceneLoader : public ISceneLoader {
private:
  Scene* m_p_scene; // Reference to the external scene

public:
  // The caller provides the scene to be filled
  SceneLoader(Scene* sceneToPopulate)
      : m_p_scene(sceneToPopulate), numShaders(0), numTextures(0) {}

  void reserveCameras(size_t count) override {
    // makes sure the vector has enough space
    //m_p_scene->cameras.reserve(count);
  }

  void reserveLights(size_t count) override {
    m_p_scene->ReservePointLights(count);
  }

  void reserveShapes(size_t count) override {
    m_p_scene->ReserveShapes(count);
  }

  void reserveShaders(size_t count) override { numShaders = count; }
  void reserveTextures(size_t count) override { numTextures = count; }

  void setSceneParams(const SceneParams &sparams) override {
    std::cout << "Setting scene params." << std::endl;
  }

  void addCamera(const std::string &name, const std::string &type,
                 const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
                 float focalLength, float imagePlaneWidth) override {
    std::cout << "Creating camera: " << name << ", type:" << type << std::endl;
    //auto cam = std::make_shared<Camera>();
    //cam->type = type;
    //m_p_scene->cameras.push_back(cam);
  }

  void addPointLight(const ISceneLoader::vec &pos,
                     const ISceneLoader::vec &intensity) override;

  void addAreaLight(const ISceneLoader::vec &pos,
                    const ISceneLoader::vec &color,
                    const ISceneLoader::vec &normal, float width,
                    float length) override {
    std::cout << "Creating area light." << std::endl;
  }

  void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override;

  void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override;

  void addTexture(const std::string &type, const std::string &name,
                  const std::string &sourceFile) override {
    std::cout << "Creating texture: type=" << type << ", name=" << name
              << ", sourceFile=" << sourceFile << std::endl;
  }

public:
  int numShaders;
  int numTextures;
  std::unordered_map<std::string, Handle<IShader>> m_ShaderMap;
};

} // namespace eng
