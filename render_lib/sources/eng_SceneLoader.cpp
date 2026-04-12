#include <engine/eng_SceneLoader.h>

/* Builtin lights. */
#include <engine/shader/shdr_PointLight.h>

#include <cassert>

namespace eng {

void SceneLoader::addPointLight(const ISceneLoader::vec &pos,
                                const ISceneLoader::vec &intensity) {
    std::cout << "Creating point light." << std::endl;
    
    /* Add the new point light to our scene. */
    m_pScene->EmplacePointLight(
        Vector3DF(pos.x, pos.y, pos.z),
        Vector3DF(intensity.x, intensity.y, intensity.z)
    );
}

void SceneLoader::addCamera(const std::string &name, const std::string &type,
               const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
               float focalLength, float imagePlaneWidth) {
    std::cout << "Creating camera: " << name << ", type:" << type << std::endl;
    
    /* We only support perspective cameras. */
    if (type != "perspective") {
        std::cout << "Invalid camera type\n";
        return;
    }

    /* Convert vectors to our native type.  */
    const Vector3DF pos_v3(pos.x, pos.y, pos.z);
    const Vector3DF view_dir_v3(viewDir.x, viewDir.y, viewDir.z);

    /* Get default image dimensions from the renderer. */
    //auto [img_width, img_height] = m_p_renderer->GetDefaultImageDimensions();

    /* Construct a new camera. */ // TODO fix dims
    auto camera = m_pObjFactory->CreatePerspectiveCamera(pos_v3, view_dir_v3, focalLength, m_defaultImgWidth, m_defaultImgHeight, imagePlaneWidth);

    /* Add it to the scene. */
    m_pScene->cameras.Insert(name, camera);
  }

void SceneLoader::addShader(const ISceneLoader::ShaderDesc &shaderDesc) {
    std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;

    /* Check if this shader already exists. */
    if (m_ShaderMap.find(shaderDesc.name) != m_ShaderMap.cend()) {
        std::cout << "shader with name " << shaderDesc.name << " already exists" << std::endl;
        return;
    }

    /* Create a shader of type type. */
    /* TODO: Move to a factory? */
    Handle<IShader> shader;
    if (shaderDesc.type == "Lambertian" || shaderDesc.type == "BlinnPhong" || shaderDesc.type == "Phong") {
        const auto& lights    = m_pScene->GetPointLights();
        const auto& diffuse = shaderDesc.diffuse.data;

        /* Create base color. */
        auto base_color = Vector3DF(diffuse.x, diffuse.y, diffuse.z);

        if (shaderDesc.type == "Lambertian")
            shader = m_pObjFactory->CreateLambertian(base_color, lights);
        else /* if (shaderDesc.type == "BlinnPhong" || shaderDesc.type == "Phong") */
            shader = m_pObjFactory->CreatePhong(base_color, lights, shaderDesc.phongExp);

    } else if (shaderDesc.type == "Mirror") {
        //shader = shdr::Mirror::Create();
    } else if (shaderDesc.type == "Diffuse") {
        /* Create diffuse shader + its base color shader. */
        const auto& diffuse = shaderDesc.diffuse.data;
        //shader = shdr::Diffuse::Create(
        //    shdr::FlatColor::Create(
        //        Vector3DF(diffuse.x, diffuse.y, diffuse.z)
        //    )
        //);
    } else if (shaderDesc.type == "Emitter") {
        const auto& color = shaderDesc.emission.data;
        //shader = shdr::Emitter::Create(Vector3DF(color.x, color.y, color.z));
    } else {
        std::cout << "Invalid shader type" << std::endl;
        return;
    }
    
    assert(shader.IsValid());
    m_ShaderMap[shaderDesc.name] = shader;
}

void SceneLoader::addShape(const ISceneLoader::ShapeDesc &shapeDesc) {
    std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;

    std::print("{}\n", shapeDesc.shaderNameReference);

    /* Find the appropriate shader. */
    Handle<IShader> shader;
    auto shader_iter = m_ShaderMap.find(shapeDesc.shaderNameReference);
    if (shader_iter != m_ShaderMap.end()) {
        shader = std::get<1>(*shader_iter);
    } else /* if (shader_iter == m_ShaderMap.end()) */ {
        std::cout << "Shader " << shapeDesc.shaderNameReference << " does not exist" << std::endl;
    }

    /* Create a shape of type. */
    Handle<IShape> new_shape;
    if (shapeDesc.type == "sphere") {
        const auto& center = shapeDesc.center;
        new_shape = m_pObjFactory->CreateSphere(Vector3DF(center.x, center.y, center.z), shapeDesc.radius, shader);
    } else if (shapeDesc.type == "triangle") {
        const auto& v0 = shapeDesc.v0, v1 = shapeDesc.v1, v2 = shapeDesc.v2;

        new_shape = m_pObjFactory->CreateTriangle(
            Vector3DF(v0.x, v0.y, v0.z),
            Vector3DF(v1.x, v1.y, v1.z),
            Vector3DF(v2.x, v2.y, v2.z),
            shader
        );
    } else {
        std::cout << "Unsupported shape type" << std::endl;
        return;
    }

    /* We must have a valid shape now. */
    assert(new_shape.IsValid());

    /* Strictly speaking, this can be caused by an incorrect json. */
    // assert(shader.IsValid());

    /* Insert the new shape and bind our shader to it. */
    m_pScene->shapes.Insert(shapeDesc.name, new_shape);
}

} // namespace eng
