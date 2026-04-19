#include <engine/eng_ObjectBase.h>
#include <engine/eng_Scene.h>

#include <engine/eng_Engine.h>

#include <engine/eng_model_obj.h>

#include <engine/framebuffer/fb_PngWriter.h>

#include <array>
#include <print>
#include <cstdio>
#include <memory>
#include <thread>
#include <vector>

using namespace eng;

constexpr int img_width  = 600;
constexpr int img_height = 600;

    constexpr float vp_height = 2.0;
    constexpr float vp_width  = 0.25;

    constexpr eng::Vector3DF pos{0, 0, 3.0};
    constexpr eng::Vector3DF dir = Vector3DF{0.0, 0, -1.0};
    constexpr float foc_len = 0.25;

    constexpr auto aspect_ratio = static_cast<float>(img_width) / img_height;

fb::Framebuffer fbb(img_width, img_height);

#ifdef __SWITCH__
#include "framebuffer/nx/NativeWindowWriter.plat-nx.h"
#endif

#define VECTOR_COLOR_IMG

int main(int argc, char** argv) {
    Engine engine;
    engine.Initialize(argc, argv);

    IObjectFactory* objectFactory = engine.GetObjectFactory();

    ModelOBJ objLoader;
    objLoader.import("dragon.obj");

    assert(objLoader.getNumberOfMeshes() > 0);
    const auto& mesh = objLoader.getMesh(0);

    std::print("{} {}\n", mesh.startIndex, mesh.triangleCount);
    std::print("{} {}\n", objLoader.getNumberOfVertices(), objLoader.getNumberOfTriangles());

    std::vector<shdr::PointLight> lights;
    lights.emplace_back(
        pos,
        Vector3DF(1,1,1)
    );

    Handle<IShader> shader = objectFactory->CreateLambertian(Vector3DF(0.5, 0.5, 0.5), lights);
    assert(shader);

    std::print("mesh triangleCount: {}\n", mesh.triangleCount);
    std::print("mesh startIndex: {}\n", mesh.startIndex);

    std::print("number of indices: {}\n", objLoader.getNumberOfIndices());
    std::print("number of vertices: {}\n", objLoader.getNumberOfVertices());
    std::print("number of triangles: {}\n", objLoader.getNumberOfTriangles());
    std::print("number of meshes: {}\n", objLoader.getNumberOfMeshes());

    std::vector<float> vertices;
    vertices.reserve(mesh.triangleCount * 9);
    const int* indexBuffer = objLoader.getIndexBuffer();
    int start = mesh.startIndex;
    int end = start + mesh.triangleCount * 3;
    for (int i = start; i < end; i++) {
        const auto& vertex = objLoader.getVertex(indexBuffer[i]);
        //std::print("{} {} {}\n", vertex.position[0], vertex.position[1], vertex.position[2]);
        vertices.push_back(vertex.position[0]);
        vertices.push_back(vertex.position[1]);
        vertices.push_back(vertex.position[2]);
    }

    std::print("vertex buffer size: {}\n", vertices.size());

    Handle<IShape> model = objectFactory->CreateMesh(vertices, Vector3DF::Zero(), shader);
    assert(model);

    std::shared_ptr<Scene> scene = engine.GetActiveScene();
    assert(scene != nullptr);

    scene->shapes.Insert("dragon", model);
    scene->shaders.Insert("lambertian", shader);

    Handle<ICamera> camera = objectFactory->CreatePerspectiveCamera(pos, dir, foc_len, img_width, img_height, vp_width);
    assert(camera);
    scene->cameras.Insert("main", camera);

    engine.RenderActiveScene("main", &fbb);

    fb::PngWriter fbWriter("out.png");
    fbWriter.WriteFramebuffer(fbb);
}
