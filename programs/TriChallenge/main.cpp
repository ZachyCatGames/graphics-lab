#include <iostream>
#include <fstream>
#include <vector>

#include <engine/eng_ObjectBase.h>
#include <engine/eng_PerspectiveCamera.h>
#include <engine/eng_Scene.h>

#include <engine/framebuffer/fb_Framebuffer.h>
#include <engine/framebuffer/fb_PngWriter.h>

#include <engine/shape/shape_Sphere.h>
#include <engine/shape/shape_Triangle.h>

#include <engine/shader/shdr_NormalShader.h>
#include <engine/shader/shdr_Lambertian.h>
#include <engine/shader/shdr_BlinnPhong.h>
#include <engine/shader/shdr_Mirror.h>
#include <engine/shader/shdr_FlatColorShader.h>

#include <engine/eng_ThreadedRenderer.h>

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

    /* Init scene with default parameters. */
    eng::Scene scene;

    /* Setup renderer. */
    eng::ThreadedRenderer renderer(&scene, img_width, img_height, 1, false, std::thread::hardware_concurrency());

    /* Setup camera. */
    renderer.EmplaceCamera<PerspectiveCamera>(pos, dir, foc_len, img_width, img_height, vp_width);

    auto shader = shdr::Lambertian::Create(
        nullptr,
        std::array{
            shdr::PointLight(Vector3DF(0, 0, 2.5), Vector3DF(1,1,1))
        }
    );
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
        scene.EmplaceShape<shape::Triangle>(
            verts[0], verts[1], verts[2]
        )
        .BindShader(shader);
    }

    /* Render through camera 0. */
    renderer.Render(0, &fbb);

    std::print("Done\n");

    fb::PngWriter png_writer("test.png");
    png_writer.WriteFramebuffer(fbb);
}
