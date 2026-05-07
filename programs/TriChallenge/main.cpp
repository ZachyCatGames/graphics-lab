#include <iostream>
#include <fstream>
#include <vector>

#include <engine/framebuffer/fb_Framebuffer.h>
#include <engine/framebuffer/fb_PngWriter.h>
#include <engine/eng_Engine.h>

#include <array>
#include <print>
#include <cstdio>
#include <memory>
#include <vector>

using namespace eng;

#include <thread>

constexpr int img_width  = 600;
constexpr int img_height = 600;

    constexpr float vp_height = 2.0;
    constexpr float vp_width  = 0.25;

    constexpr eng::Vector3DF pos{0, 0, 3.0};
    constexpr eng::Vector3DF dir = Vector3DF{0.0, 0, -1.0};
    constexpr float foc_len = 0.25;

    constexpr auto aspect_ratio = static_cast<float>(img_width) / img_height;


fb::Framebuffer fbb(img_width, img_height);

void readFloatsFromFile(const std::string& filename, std::vector<float> &allFloats)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return;
    }

    float value;
    char c;

    while (file >> value) {
        allFloats.push_back(value);

        // Consume comma if there
        if (file.peek() == ',') file.get(c);
    }
}

int main(int argc, char* argv[])
{
    std::vector<float> allFloats;
    readFloatsFromFile( "trilist.dat", allFloats );

    std::cout << "Number of floats: " << allFloats.size() << std::endl;
    int numVertices = allFloats.size() / 3;
    std::cout << "Interpreted as Vertices: " << numVertices << std::endl;
    int numTriangles = numVertices / 3;
    std::cout << "Interpreted as Triangles: " << numTriangles << std::endl;

    //auto shader = eng::BlinnPhongShader::Create(eng::ray{ eng::Vector3DF{ 0,0,0 }, eng::Vector3DF(3, 4,5)}, 20.0);
    //auto shader = eng::shdr::Lambertian::Create(eng::Ray{ eng::Vector3DF{ 0,0,0 }, eng::Vector3DF(3, 4,5)});

    Engine engine;
    engine.Initialize(argc, argv);

    /* Get the object factory. */
    IObjectFactory* pObjFactory = engine.GetObjectFactory();

    /* Get the current active scene. */
    std::shared_ptr<Scene> pScene = engine.GetActiveScene();

    /* Setup camera. */
    Handle<ICamera> camera = pObjFactory->CreatePerspectiveCamera(pos, dir, foc_len, img_width, img_height, vp_width);

    /* Setup lambertian shader. */
    Handle<IShader> shader = pObjFactory->CreateLambertian(
        Vector3DF(1,1,1),
        std::vector{
            shdr::PointLight(Vector3DF(0, 0, 2.5), Vector3DF(1,1,1))
        }
    );

    /* Add everything to the scene. */
    pScene->shaders.Insert("lambertian", shader);
    pScene->cameras.Insert("main", camera);

    for (int i = 0; i < numTriangles; i++) {
        Vector3DF verts[3];
        for (int j = 0; j < 3; j++) {
            verts[j] = Vector3DF(
                allFloats[i * 9 + j * 3],
                allFloats[i * 9 + j * 3 + 1],
                allFloats[i * 9 + j * 3 + 2]
            );

            //std::clog << verts[j] << '\n';

        }

        Handle<IShape> triangle = pObjFactory->CreateTriangle(verts[0], verts[1], verts[2], shader);
        pScene->shapes.Insert(triangle);
    }

    /* Render through camera 0. */
    engine.RenderActiveScene("main", &fbb);

    std::print("Done\n");

    fb::PngWriter png_writer("test.png");
    png_writer.WriteFramebuffer(fbb);
}
